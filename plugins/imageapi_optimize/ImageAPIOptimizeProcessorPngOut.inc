<?php

class ImageAPIOptimizeProcessorPngOut extends ImageAPIOptimizeProcessorBinary implements ImageAPIOptimizeProcessorInterface {

  protected function executableName() {
    return 'pngout';
  }


  public function getDescription() {
    return '';
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
      );
      $arguments = array(
        $dst,
      );

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
