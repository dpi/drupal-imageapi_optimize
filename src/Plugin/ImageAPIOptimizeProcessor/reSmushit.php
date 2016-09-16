<?php

namespace Drupal\imageapi_optimize\Plugin\ImageAPIOptimizeProcessor;

use Drupal\Core\Image\ImageInterface;
use Drupal\imageapi_optimize\ImageAPIOptimizeProcessorBase;

/**
 * Desaturates (grayscale) an image resource.
 *
 * @ImageAPIOptimizeProcessor(
 *   id = "resmushit",
 *   label = @Translation("Resmush.it"),
 *   description = @Translation("Uses the free resmush.it service to optimize images.")
 * )
 */
class reSmushit extends ImageAPIOptimizeProcessorBase {

  public function applyToImage($image_uri) {
    // @TODO: implement this.
    drupal_set_message('Optimize');
    return TRUE;
  }

}
