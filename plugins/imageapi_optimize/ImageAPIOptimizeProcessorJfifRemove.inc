<?php

class ImageAPIOptimizeProcessorJfifRemove extends ImageAPIOptimizeProcessorBinary implements ImageAPIOptimizeProcessorInterface {

  protected function executableName() {
    return 'jfifremove';
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
