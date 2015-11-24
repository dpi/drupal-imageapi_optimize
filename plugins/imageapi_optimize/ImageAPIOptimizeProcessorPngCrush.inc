<?php

class ImageAPIOptimizeProcessorPngCrush extends ImageAPIOptimizeProcessorBinary implements ImageAPIOptimizeProcessorInterface {

  protected function executableName() {
    return 'pngcrush';
  }


  public function getDescription() {
    return '';
  }

  public function process($image, $destination) {
    if ($this->getManualExecutablePath()) {
      $cmd = $this->getManualExecutablePath();
    }
    elseif ($this->findExecutablePath()) {
      $cmd = $this->findExecutablePath();
    }

    if (empty($cmd)) {
      return FALSE;
    }
    $dst = $this->sanitizeFilename($destination);

    if ($image->info['mime_type'] == 'image/png') {
      $temp = drupal_realpath(drupal_tempnam('temporary://', 'file'));
      $options = array(
        '-rem alla',
        '-reduce',
        '-brute',
        '-q'
      );
      $arguments = array(
        $dst,
        $temp,
      );

      $option_string = implode(' ', $options);
      $argument_string = implode(' ', array_map('escapeshellarg', $arguments));
      exec("$cmd " . $option_string . ' ' . $argument_string . ' && mv ' . escapeshellarg($temp) . ' ' . escapeshellarg($dst), $output, $return_val);

      if ($return_val == 0) {
        return TRUE;
      }
    }
    return;
  }
}
