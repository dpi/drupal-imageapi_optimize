<?php

namespace Drupal\iamgeapi_optimize;

interface ImageAPIOptimizeShellOperationsInterface {
  public function findExecutablePath($executable = NULL);
  public function execShellCommand($command, $options, $arguments);
  public function saveCommandStdoutToFile($cmd, $dst);
}
