<?php

class ImageAPIOptimizeProcessorPngOut extends ImageAPIOptimizeProcessorBinary implements ImageAPIOptimizeProcessorInterface {

  protected function executableName() {
    return 'pngout';
  }

  public function process($image, $destination) {
    $cmd = $this->getFullPathToBinary();

    if (empty($cmd)) {
      return FALSE;
    }
    $dst = $this->sanitizeFilename($destination);

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
