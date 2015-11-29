<?php

class ImageAPIOptimizeProcessorAdvPng extends ImageAPIOptimizeProcessorBinary implements ImageAPIOptimizeProcessorConfigurableInterface {

  protected $recompress = TRUE;
  protected $mode = 3;

  /**
   * ImageAPIOptimizeProcessorReSmushIt constructor.
   */
  public function __construct($data) {
    parent::__construct($data);
    if (isset($data['recompress'])) {
      $this->recompress = $data['recompress'];
    }
    if (isset($data['mode'])) {
      $this->mode = $data['mode'];
    }
  }

  /**
   * @return boolean
   */
  public function isRecompress() {
    return $this->recompress;
  }

  /**
   * @return int
   */
  public function getMode() {
    return $this->mode;
  }

  protected function executableName() {
    return 'advpng';
  }

  public function configForm() {

    $form = parent::configForm();

    $form['recompress'] = array(
      '#title' => t('Recompress'),
      '#type' => 'checkbox',
      '#default_value' => $this->isRecompress(),
    );

    $form['mode'] = array(
      '#title' => t('Compression mode'),
      '#type' => 'select',
      '#options' => array(
        0 => t('Disabled'),
        1 => t('Fast'),
        2 => t('Normal'),
        3 => t('Extra'),
        4 => t('Insane'),
      ),
      '#default_value' => $this->getMode(),
    );

    return $form;
  }

  public function process($image, $destination) {
    $cmd = $this->getFullPathToBinary();

    if (empty($cmd)) {
      return FALSE;
    }
    $dst = $this->sanitizeFilename($destination);

    if ($image->info['mime_type'] == 'image/png') {
      $options = array(
        '--quiet',
      );
      $arguments = array(
        $dst,
      );

      if ($this->isRecompress()) {
        $options[] = '--recompress';
      }

      $options[] = '-' . $this->getMode();

      $option_string = implode(' ', $options);
      $argument_string = implode(' ', array_map('escapeshellarg', $arguments));
      exec("$cmd " . $option_string . ' ' . $argument_string, $output, $return_val);

      if ($return_val == 0) {
        return TRUE;
      }
    }
    return;
  }
}
