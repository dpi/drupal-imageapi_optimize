<?php

namespace Drupal\imageapi_optimize;

use Drupal\Core\Config\Entity\ConfigEntityInterface;

/**
 * Provides an interface defining an image style entity.
 */
interface ImageAPIOptimizePipelineInterface extends ConfigEntityInterface {

  /**
   * Returns the replacement ID.
   *
   * @return string|null
   *   The replacement image style ID or NULL if no replacement has been
   *   selected.
   *
   * @deprecated in Drupal 8.0.x, will be removed before Drupal 9.0.x. Use
   *   \Drupal\imageapi_optimize\ImageStyleStorageInterface::getReplacementId() instead.
   *
   * @see \Drupal\imageapi_optimize\ImageStyleStorageInterface::getReplacementId()
   */
  public function getReplacementID();

  /**
   * Returns the image style.
   *
   * @return string
   *   The name of the image style.
   */
  public function getName();

  /**
   * Sets the name of the image style.
   *
   * @param string $name
   *   The name of the image style.
   *
   * @return \Drupal\imageapi_optimize\ImageStyleInterface
   *   The class instance this method is called on.
   */
  public function setName($name);

  /**
   * Returns a specific image effect.
   *
   * @param string $effect
   *   The image effect ID.
   *
   * @return \Drupal\imageapi_optimize\ImageAPIOptimizeProcessorInterface
   *   The image effect object.
   */
  public function getProcessor($processor);

  /**
   * Returns the image effects for this style.
   *
   * @return \Drupal\imageapi_optimize\ImageAPIOptimizeProcessorPluginCollection|\Drupal\imageapi_optimize\ImageAPIOptimizeProcessorInterface[]
   *   The image effect plugin collection.
   */
  public function getProcessors();

  /**
   * Saves an image effect for this style.
   *
   * @param array $configuration
   *   An array of image effect configuration.
   *
   * @return string
   *   The image effect ID.
   */
  public function addProcessor(array $configuration);

  /**
   * Deletes an image effect from this style.
   *
   * @param \Drupal\imageapi_optimize\ImageAPIOptimizeProcessorInterface $effect
   *   The image effect object.
   *
   * @return $this
   */
  public function deleteProcessor(ImageAPIOptimizeProcessorInterface $effect);

  /**
   * Flushes cached media for this pipeline.
   *
   * @return $this
   */
  public function flush();

}
