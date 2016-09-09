<?php

namespace Drupal\imageapi_optimize\Annotation;

use Drupal\Component\Annotation\Plugin;

/**
 * Defines an image effect annotation object.
 *
 * Plugin Namespace: Plugin\ImageEffect
 *
 * For a working example, see
 * \Drupal\imageapi_optimize\Plugin\ImageEffect\ResizeImageEffect
 *
 * @see hook_image_effect_info_alter()
 * @see \Drupal\imageapi_optimize\ConfigurableImageEffectInterface
 * @see \Drupal\imageapi_optimize\ConfigurableImageEffectBase
 * @see \Drupal\imageapi_optimize\ImageEffectInterface
 * @see \Drupal\imageapi_optimize\ImageEffectBase
 * @see \Drupal\imageapi_optimize\ImageEffectManager
 * @see \Drupal\Core\ImageToolkit\Annotation\ImageToolkitOperation
 * @see plugin_api
 *
 * @Annotation
 */
class ImageAPIOptimizeProcessor extends Plugin {

  /**
   * The plugin ID.
   *
   * @var string
   */
  public $id;

  /**
   * The human-readable name of the image effect.
   *
   * @ingroup plugin_translatable
   *
   * @var \Drupal\Core\Annotation\Translation
   */
  public $label;

  /**
   * A brief description of the image effect.
   *
   * This will be shown when adding or configuring this image effect.
   *
   * @ingroup plugin_translatable
   *
   * @var \Drupal\Core\Annotation\Translation (optional)
   */
  public $description = '';

}
