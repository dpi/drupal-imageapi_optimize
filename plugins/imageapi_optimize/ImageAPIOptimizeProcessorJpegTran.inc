<?php

class ImageAPIOptimizeProcessorJpegTran extends ImageAPIOptimizeProcessorBinary implements ImageAPIOptimizeProcessorConfigurableInterface {

  protected $progressive = FALSE;


  /**
   * ImageAPIOptimizeProcessorReSmushIt constructor.
   */
  public function __construct($data) {
    parent::__construct($data);
    if (isset($data['progressive'])) {
      $this->progressive = $data['progressive'];
    }
  }

  /**
   * @return string
   */
  public function getProgressive() {
    return $this->progressive;
  }

  protected function executableName() {
    return 'jpegtran';
  }

  public function configForm() {

    $form = parent::configForm();

    $form['progressive'] = array(
      '#title' => t('Progressive'),
      '#type' => 'checkbox',
      '#default_value' => $this->getProgressive(),
    );

    return $form;
  }

  public function process($image, $destination) {
    $cmd = $this->getFullPathToBinary();

    if (empty($cmd)) {
      return FALSE;
    }
    $dst = $this->sanitizeFilename($destination);

    if ($image->info['mime_type'] == 'image/jpeg') {
      $options = array(
        '-copy none',
        '-optimize',
      );
      $arguments = array(
        $dst,
      );

      if ($this->getProgressive()) {
        $options[] = '-progressive';
      }

      $option_string = implode(' ', $options);
      $argument_string = implode(' ', array_map('escapeshellarg', $arguments));
      $this->saveCommandStdoutToFile("$cmd " . $option_string . ' ' . $argument_string, $dst);
    }
    return;
  }
}
