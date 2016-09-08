<?php

class ImageAPIOptimizeProcessorJfifRemove extends ImageAPIOptimizeProcessorBinary implements ImageAPIOptimizeProcessorInterface {

  protected function executableName() {
    return 'jfifremove';
  }

  public function process($image, $destination) {
    $cmd = $this->getFullPathToBinary();

    if (empty($cmd)) {
      return FALSE;
    }
    $dst = $this->sanitizeFilename($destination);

    if ($image->info['mime_type'] == 'image/jpeg') {
      $options = array();
      $arguments = array(
        $dst,
      );

      $option_string = implode(' ', $options);
      $argument_string = implode(' ', array_map('escapeshellarg', $arguments));
      $this->saveCommandStdoutToFile("$cmd " . $option_string . ' < ' . $argument_string, $dst);
    }
    return;
  }
}
