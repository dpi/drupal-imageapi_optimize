<?php

class ImageAPIOptimizeProcessorKraken implements ImageAPIOptimizeProcessorConfigurableInterface {

  protected $api_key;
  protected $api_secret;
  protected $lossy = TRUE;

  /**
   * ImageAPIOptimizeProcessorReSmushIt constructor.
   */
  public function __construct($data) {
    if (isset($data['api_key'])) {
      $this->api_key = $data['api_key'];
    }
    if (isset($data['api_secret'])) {
      $this->api_secret = $data['api_secret'];
    }
    if (isset($data['lossy'])) {
      $this->lossy = $data['lossy'];
    }
  }

  public function configForm() {
    if (!$this->libraryPresent()) {
      $form['library'] = array(
        '#type' => 'markup',
        '#markup' => t('Kraken.io integration requires the
        <a href="">Kraken-PHP library</a> to be installed in your libraries folder as kraken-php (sites/all/libraries/kraken-php).', array(
            '!kraken-lib' => l(t('Kraken-PHP library'), 'https://github.com/kraken-io/kraken-php'),
          )
        ),
      );
    }
    else {
      $form['api_key'] = array(
        '#title' => t('API Key'),
        '#type' => 'textfield',
        '#required' => TRUE,
        '#default_value' => $this->api_key,
      );

      $form['api_secret'] = array(
        '#title' => t('API Secret'),
        '#type' => 'textfield',
        '#required' => TRUE,
        '#default_value' => $this->api_secret,
      );

      $form['lossy'] = array(
        '#title' => t('Use lossy compression'),
        '#type' => 'checkbox',
        '#default_value' => $this->lossy,
      );
    }

    return $form;
  }

  /**
   * @return mixed
   */
  public function getApiKey() {
    return $this->api_key;
  }

  /**
   * @return mixed
   */
  public function getApiSecret() {
    return $this->api_secret;
  }

  /**
   * @return boolean
   */
  public function isLossy() {
    return $this->lossy;
  }

  public function getDescription() {
    // Check to see if we can contact the API.
    if (!$this->libraryPresent()) {
      return t('<strong>Kraken PHP library not present.</strong>');
    }
    else {
      if ($this->isLossy()) {
        return t('Using lossy compression.');
      }
      else {
        return t('Using lossless compression.');
      }
    }
  }

  /**
   * Try to detect the kraken library.
   *
   * @return bool
   */
  protected function libraryPresent() {
    $libraries = libraries_detect('kraken-php');
    if (isset($libraries['error'])) {
      return FALSE;
    }

    if (!libraries_load('kraken-php')) {
      return FALSE;
    }

    return TRUE;
  }

  public function process($image, $dst) {
    $real_path = drupal_realpath($dst);

    if (!$this->libraryPresent()) {
      return FALSE;
    }

    if ($this->getApiKey() && $this->getApiSecret()) {
      $kraken = new Kraken($this->getApiKey(), $this->getApiSecret());
      $params = array(
        'file' => $real_path,
        'wait' => TRUE,
        'lossy' => (bool) $this->isLossy(),
      );

      // Send the request to Kraken.
      $data = $kraken->upload($params);

      if (!empty($data['success']) && !empty($data['kraked_url'])) {
        $result = drupal_http_request($data['kraked_url']);
        if (!isset($result->error)) {
          file_unmanaged_save_data($result->data, $dst, FILE_EXISTS_REPLACE);
          // @todo: check to see if watchdog entries are enabled for kraken
          watchdog('kraken.io', '@file_name was successfully processed by Kraken.io.
        Original size: @original_size; Kraked size: @kraked_size; Total saved:
        @saved_bytes. All figures in bytes', array(
              '@file_name' => $data['file_name'],
              '@original_size' => $data['original_size'],
              '@kraked_size' => $data['kraked_size'],
              '@saved_bytes' => $data['saved_bytes'],
            )
          );
          return TRUE;
        }
      }
    }
  }
}
