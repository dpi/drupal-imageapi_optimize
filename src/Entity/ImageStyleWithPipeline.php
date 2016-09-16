<?php

namespace Drupal\imageapi_optimize\Entity;

use Drupal\image\Entity\ImageStyle;

class ImageStyleWithPipeline extends ImageStyle {

  protected $pipeline;

  public function createDerivative($original_uri, $derivative_uri) {
    $result = parent::createDerivative($original_uri, $derivative_uri);
    if ($result) {
      // Apply the pipeline to the $derivative_uri.
      if ($this->hasPipeline()) {
        $this->getPipeline()->applyToImage($derivative_uri);
      }
    }
  }

  /**
   * @return \Drupal\imageapi_optimize\Entity\ImageAPIOptimizePipeline|null
   */
  public function getPipeline() {
    if (!empty($this->pipeline)) {
      $storage = $this->entityTypeManager()->getStorage('imageapi_optimize_pipeline');
      if ($pipeline = $storage->load($this->pipeline)) {
        return $pipeline;
      }
    }
  }

  public function hasPipeline() {
    return (bool) $this->getPipeline();
  }


}