<?php

class ImageAPIOptimizeProcessorJpegOptim extends ImageAPIOptimizeProcessorBinary implements ImageAPIOptimizeProcessorConfigurableInterface {

  protected $progressive = '';
  protected $size = '';
  protected $quality = '';


  /**
   * ImageAPIOptimizeProcessorReSmushIt constructor.
   */
  public function __construct($data) {
    parent::__construct($data);
    if (isset($data['progressive'])) {
      $this->progressive = $data['progressive'];
    }
    if (isset($data['quality'])) {
      $this->quality = $data['quality'];
    }
    if (isset($data['size'])) {
      $this->size = $data['size'];
    }
  }

  /**
   * @return string
   */
  public function getProgressive() {
    return $this->progressive;
  }

  /**
   * @return string
   */
  public function getSize() {
    return $this->size;
  }

  /**
   * @return string
   */
  public function getQuality() {
    return $this->quality;
  }



  protected function executableName() {
    return 'jpegoptim';
  }


  public function getDescription() {
    return '';
  }

  public function configForm() {

    $form = parent::configForm();

    $form['progressive'] = array(
      '#title' => t('Progressive'),
      '#type' => 'select',
      '#options' => array(
        '' => t('No change'),
        0 => t('Non-progressive'),
        1 => t('Progressive'),
      ),
      '#default_value' => $this->getProgressive(),
      '#description' => t('If "No change" is select, the output will have the same as the input.'),
    );

    $form['quality'] = array(
      '#title' => t('Quality'),
      '#type' => 'textfield',
      '#size' => 10,
      '#element_validate' => array('element_validate_integer_positive'),
      '#description' => t('Optionally enter a JPEG quality setting to use, 0 - 100. WARNING: LOSSY'),
      '#default_value' => $this->getQuality(),
    );

    $form['size'] = array(
      '#title' => t('Target size'),
      '#type' => 'textfield',
      '#size' => 10,
      '#element_validate' => array('element_validate_integer_positive'),
      '#field_suffix' => '%',
      '#description' => t('Optionally enter a target percentage of filesize for optimisation. WARNING: LOSSY'),
      '#default_value' => $this->getSize(),
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

    if ($image->info['mime_type'] == 'image/jpeg') {
      $options = array(
        '--quiet',
        '--strip-all',
      );
      $arguments = array(
        $dst,
      );

      if (is_numeric($this->getProgressive())) {
        switch ($this->getProgressive()) {
          case 0:
            $options[] = '--all-normal';
            break;

          case 1:
            $options[] = '--all-progressive';
            break;
        }
      }

      if (is_numeric($this->getQuality())) {
        $options[] = '--max=' . escapeshellarg($this->getQuality());
      }

      if (is_numeric($this->getSize())) {
        $options[] = '--size=' . escapeshellarg($this->getSize() . '%');
      }

      $output = array();
      $return_val = 0;
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
