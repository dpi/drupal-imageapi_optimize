<?php

namespace Drupal\imageapi_optimize;

use Drupal\Core\Plugin\PluginFormInterface;

/**
 * Defines the interface for configurable image optimize processors.
 *
 * @see \Drupal\imageapi_optimize\Annotation\ImageEffect
 * @see \Drupal\imageapi_optimize\ConfigurableImageEffectBase
 * @see \Drupal\imageapi_optimize\ImageEffectInterface
 * @see \Drupal\imageapi_optimize\ImageEffectBase
 * @see \Drupal\imageapi_optimize\ImageEffectManager
 * @see plugin_api
 */
interface ConfigurableImageAPIOptimizeProcessorInterface extends ImageAPIOptimizeProcessorInterface, PluginFormInterface {
}
