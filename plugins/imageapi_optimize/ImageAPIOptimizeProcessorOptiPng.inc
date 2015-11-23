<?php

class ImageAPIOptimizeProcessorOptiPng extends ImageAPIOptimizeProcessorBinary implements ImageAPIOptimizeProcessorConfigurableInterface {

  protected $level = 5;
  protected $interlace = '';

  /**
   * ImageAPIOptimizeProcessorReSmushIt constructor.
   */
  public function __construct($data) {
    parent::__construct($data);
    if (isset($data['level'])) {
      $this->level = $data['level'];
    }
    if (isset($data['interlace'])) {
      $this->interlace = $data['interlace'];
    }
  }

  /**
   * @return int
   */
  public function getLevel() {
    return $this->level;
  }

  /**
   * @return string
   */
  public function getInterlace() {
    return $this->interlace;
  }

  protected function executableName() {
    return 'optipng';
  }


  public function getDescription() {
    return '';
  }

  public function configForm() {

    $form = parent::configForm();

    $form['level'] = array(
      '#title' => t('Optimization level'),
      '#type' => 'select',
      '#options' => range(0, 7),
      '#default_value' => $this->getLevel(),
    );

    $form['interlace'] = array(
      '#title' => t('Interlace'),
      '#type' => 'select',
      '#options' => array(
        '' => t('No change'),
        0 => t('Non-interlaced'),
        1 => t('Interlaced'),
      ),
      '#default_value' => $this->getInterlace(),
      '#description' => t('If "No change" is select, the output will have the same interlace type as the input.'),
    );

    return $form;
  }

  public function process($image, $dst) {
    if ($this->getManualExecutablePath()) {
      $cmd = $this->getManualExecutablePath();
    }
    elseif ($this->findExecutablePath()) {
      $cmd = $this->findExecutablePath();
    }

    if (empty($cmd)) {
      return FALSE;
    }

    if ($image->info['mime_type'] == 'image/png') {
      $options = array(
        '--quiet',
      );
      $arguments = array(
        $dst,
      );

      if (is_numeric($this->getInterlace())) {
        $options[] = '-i ' . $this->getInterlace();
      }

      $options[] = '-o' . $this->getLevel();

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
