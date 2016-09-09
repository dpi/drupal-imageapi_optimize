<?php

namespace Drupal\imageapi_optimize;

use Drupal\Component\Plugin\ConfigurablePluginInterface;
use Drupal\Component\Plugin\PluginInspectionInterface;
use Drupal\imageapi_optimize\ImageInterface;

/**
 * Defines the interface for image effects.
 *
 * @see \Drupal\imageapi_optimize\Annotation\ImageEffect
 * @see \Drupal\imageapi_optimize\ImageEffectBase
 * @see \Drupal\imageapi_optimize\ConfigurableImageEffectInterface
 * @see \Drupal\imageapi_optimize\ConfigurableImageEffectBase
 * @see \Drupal\imageapi_optimize\ImageEffectManager
 * @see plugin_api
 */
interface ImageAPIOptimizeProcessorInterface extends PluginInspectionInterface, ConfigurablePluginInterface {

  /**
   * Returns a render array summarizing the configuration of the image effect.
   *
   * @return array
   *   A render array.
   */
  public function getSummary();

  /**
   * Returns the image effect label.
   *
   * @return string
   *   The image effect label.
   */
  public function label();

  /**
   * Returns the unique ID representing the image effect.
   *
   * @return string
   *   The image effect ID.
   */
  public function getUuid();

  /**
   * Returns the weight of the image effect.
   *
   * @return int|string
   *   Either the integer weight of the image effect, or an empty string.
   */
  public function getWeight();

  /**
   * Sets the weight for this image effect.
   *
   * @param int $weight
   *   The weight for this image effect.
   *
   * @return $this
   */
  public function setWeight($weight);

}
