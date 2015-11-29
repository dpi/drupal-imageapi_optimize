<?php

class ImageAPIOptimizeProcessorReSmushIt implements ImageAPIOptimizeProcessorConfigurableInterface {

  protected $connectionTimeout = 5;

  /**
   * ImageAPIOptimizeProcessorReSmushIt constructor.
   */
  public function __construct($data) {
    if (isset($data['connection_timeout'])) {
      $this->connectionTimeout = $data['connection_timeout'];
    }
  }

  /**
   * @return int
   */
  public function getConnectionTimeout() {
    return $this->connectionTimeout;
  }

  public function configForm() {
    $form['connection_timeout'] = array(
      '#type' => 'textfield',
      '#default_value' => $this->getConnectionTimeout(),
      '#title' => t('Connection timeout'),
      '#description' => t('Number of seconds to wait before giving up waiting for resmush.it to return an optimized image.'),
      '#field_suffix' => 'seconds',
      '#required' => TRUE,
      '#size' => 4,
      '#maxlength' => 3,
      '#element_validate' => array('imageapi_optimize_processor_integer_validate'),
      '#allow_negative' => FALSE,
    );

    return $form;
  }

  public function getDescription() {
    return format_plural($this->getConnectionTimeout(), 'Connection timeout: 1 second.', 'Connection timeout: @count seconds.');
  }

  public function process($image, $dst) {
    $dst = drupal_realpath($dst);
    $url = 'http://www.resmush.it/ws.php';
    $ch = curl_init();
    curl_setopt($ch, CURLOPT_URL, $url);
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE);
    curl_setopt($ch, CURLOPT_CONNECTTIMEOUT, 5);
    curl_setopt($ch, CURLOPT_POST, TRUE);
    if (!class_exists('CURLFile')) {
      $arg = array('files' => '@' . drupal_realpath($dst));
    }
    else {
      $cfile = new CURLFile($dst);
      $arg = array(
        'files' => $cfile,
      );
    }
    curl_setopt($ch, CURLOPT_POSTFIELDS, $arg);

    $data = curl_exec($ch);
    curl_close($ch);

    if ($data && ($json = json_decode($data))) {
      // reSmushIt returns an error if it cannot optimize the image. Otherwise, it
      // returns an object, with 'dest' (temporary file) and 'percent' (savings)
      // among other properties.
      if (!isset($json->error) && isset($json->dest)) {
        $result = drupal_http_request($json->dest);
        if (!isset($result->error)) {
          file_unmanaged_save_data($result->data, $dst, FILE_EXISTS_REPLACE);
          return TRUE;
        }
      }
    }
  }
}
