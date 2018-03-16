<?php

namespace Drupal\imageapi_optimize;

interface ImageAPIOptimizeShellOperationsInterface {
  public function findExecutablePath($executable = NULL);
  public function execShellCommand($command, $options, $arguments);
  public function saveCommandStdoutToFile($cmd, $dst);
}
