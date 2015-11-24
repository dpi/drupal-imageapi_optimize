<?php

abstract class ImageAPIOptimizeProcessorBinary {
  protected $manual_executable_path;

  /**
   * ImageAPIOptimizeProcessorReSmushIt constructor.
   */
  public function __construct($data) {
    if (isset($data['manual_executable_path'])) {
      $this->manual_executable_path = $data['manual_executable_path'];
    }
  }

  /**
   * @return mixed
   */
  public function getManualExecutablePath() {
    return $this->manual_executable_path;
  }

  abstract protected function executableName();

  public function configForm() {

    if (!$this->findExecutablePath()) {
      $form['executable'] = array(
        '#type' => 'item',
        '#title' => t('Executable'),
        '#markup' => t('The @binary binary must be installed, please install or specify the path to the @binary executable directly.', array('@binary' => $this->executableName())),
      );
    }
    else {
      $form['executable'] = array(
        '#type' => 'item',
        '#title' => t('Executable'),
        '#markup' => t('The @binary executable has been automatically located here: @path. To override, set a different executate path below.', array('@path' => $this->findExecutablePath(), '@binary' => $this->executableName())),
      );
    }

    $form['manual_executable_path'] = array(
      '#type' => 'textfield',
      '#title' => t('Manually set path'),
      '#description' => t('Specify the full path to the @binary executable.', array('@binary' => $this->executableName())),
      '#default_value' => $this->getManualExecutablePath(),
    );

    return $form;
  }

  protected function findExecutablePath($executable = NULL) {
    if (is_null($executable)) {
      $executable = $this->executableName();
    }
    $output = array();
    $return_var = 0;
    $path = exec('which ' . escapeshellarg($executable), $output, $return_var);
    if ($return_var == 0) {
      return $path;
    }
  }

  /**
   * Sanitize the given path for binary processors.
   *
   * @param $drupal_filepath
   *   The file path to sanitize.
   *
   * @return string
   *   The sanitized file path.
   */
  protected function sanitizeFilename($drupal_filepath) {
    return drupal_realpath($drupal_filepath);
  }

  protected function saveCommandStdoutToFile($cmd, $dst) {
    ob_start();
    passthru($cmd);
    $output = ob_get_contents();
    ob_end_clean();

    file_unmanaged_save_data($output, $dst, FILE_EXISTS_REPLACE);
  }
}