<?php

namespace Drupal\imageapi_optimize\Entity;

use Drupal\Core\Cache\Cache;
use Drupal\Core\Config\Entity\ConfigEntityBase;
use Drupal\Core\Entity\EntityStorageInterface;
use Drupal\Core\Entity\EntityWithPluginCollectionInterface;
use Drupal\Core\Routing\RequestHelper;
use Drupal\Core\Site\Settings;
use Drupal\Core\Url;
use Drupal\imageapi_optimize\ImageAPIOptimizeProcessorPluginCollection;
use Drupal\imageapi_optimize\ImageAPIOptimizeProcessorInterface;
use Drupal\imageapi_optimize\ImageAPIOptimizePipelineInterface;
use Drupal\Component\Utility\Crypt;
use Drupal\Component\Utility\UrlHelper;
use Drupal\Core\StreamWrapper\StreamWrapperInterface;
use Symfony\Component\DependencyInjection\Exception\ServiceNotFoundException;
use Drupal\Core\Entity\Entity\EntityViewDisplay;
/**
 * Defines an image style configuration entity.
 *
 * @ConfigEntityType(
 *   id = "imageapi_optimize_pipeline",
 *   label = @Translation("ImageAPI Optimize Pipeline"),
 *   handlers = {
 *     "form" = {
 *       "add" = "Drupal\imageapi_optimize\Form\ImageAPIOptimizePipelineAddForm",
 *       "edit" = "Drupal\imageapi_optimize\Form\ImageAPIOptimizePipelineEditForm",
 *       "delete" = "Drupal\imageapi_optimize\Form\ImageAPIOptimizePipelineDeleteForm",
 *       "flush" = "Drupal\imageapi_optimize\Form\ImageAPIOptimizePipelineFlushForm"
 *     },
 *     "list_builder" = "Drupal\imageapi_optimize\ImageAPIOptimizePipelineListBuilder",
 *     "storage" = "Drupal\imageapi_optimize\ImageAPIOptimizePipelineStorage",
 *   },
 *   admin_permission = "administer imageapi optimize pipelines",
 *   config_prefix = "processor",
 *   entity_keys = {
 *     "id" = "name",
 *     "label" = "label"
 *   },
 *   links = {
 *     "flush-form" = "/admin/config/media/imageapi-optimize-pipelines/manage/{imageapi_optimize_pipeline}/flush",
 *     "edit-form" = "/admin/config/media/imageapi-optimize-pipelines/manage/{imageapi_optimize_pipeline}",
 *     "delete-form" = "/admin/config/media/imageapi-optimize-pipelines/manage/{imageapi_optimize_pipeline}/delete",
 *     "collection" = "/admin/config/media/imageapi-optimize-pipelines",
 *   },
 *   config_export = {
 *     "name",
 *     "label",
 *     "processors",
 *   }
 * )
 */
class ImageAPIOptimizePipeline extends ConfigEntityBase implements ImageAPIOptimizePipelineInterface, EntityWithPluginCollectionInterface {

  /**
   * The name of the image style.
   *
   * @var string
   */
  protected $name;

  /**
   * The image style label.
   *
   * @var string
   */
  protected $label;

  /**
   * The array of image effects for this image style.
   *
   * @var array
   */
  protected $processors = array();

  /**
   * Holds the collection of image effects that are used by this image style.
   *
   * @var \Drupal\imageapi_optimize\ImageAPIOptimizeProcessorPluginCollection
   */
  protected $processorsCollection;

  /**
   * {@inheritdoc}
   */
  public function id() {
    return $this->name;
  }

  /**
   * {@inheritdoc}
   */
  public function postSave(EntityStorageInterface $storage, $update = TRUE) {
    parent::postSave($storage, $update);

    if ($update) {
      if (!empty($this->original) && $this->id() !== $this->original->id()) {
        // The old image style name needs flushing after a rename.
        $this->original->flush();
        // Update field settings if necessary.
        if (!$this->isSyncing()) {
          static::replaceImageAPIOptimizePipeline($this);
        }
      }
      else {
        // Flush image style when updating without changing the name.
        $this->flush();
      }
    }
  }

  /**
   * {@inheritdoc}
   */
  public static function postDelete(EntityStorageInterface $storage, array $entities) {
    parent::postDelete($storage, $entities);

    /** @var \Drupal\imageapi_optimize\ImageAPIOptimizePipelineInterface[] $entities */
    foreach ($entities as $pipeline) {
      // Flush cached media for the deleted pipeline.
      $pipeline->flush();
      // Clear the replacement ID, if one has been previously stored.
      /** @var \Drupal\imageapi_optimize\ImageAPIOptimizePipelineStorageInterface $storage */
      $storage->clearReplacementId($pipeline->id());
    }
  }

  /**
   * Update field settings if the image style name is changed.
   *
   * @param \Drupal\imageapi_optimize\ImageAPIOptimizePipelineInterface $style
   *   The image style.
   */
  protected static function replaceImageAPIOptimizePipeline(ImageAPIOptimizePipelineInterface $style) {
    if ($style->id() != $style->getOriginalId()) {
      // Loop through all image styles looking for usages.

    }
  }

  /**
   * {@inheritdoc}
   */
  public function flush() {

    // Let other modules update as necessary on flush.
    $module_handler = \Drupal::moduleHandler();
    $module_handler->invokeAll('imageapi_optimize_pipeline_flush', array($this));

    Cache::invalidateTags($this->getCacheTagsToInvalidate());

    return $this;
  }

  /**
   * {@inheritdoc}
   */
  // @TODO: change this to our main pipeline apply function.
  public function createDerivative($original_uri, $derivative_uri) {
//
//    // If the source file doesn't exist, return FALSE without creating folders.
//    $image = \Drupal::service('image.factory')->get($original_uri);
//    if (!$image->isValid()) {
//      return FALSE;
//    }
//
//    // Get the folder for the final location of this style.
//    $directory = drupal_dirname($derivative_uri);
//
//    // Build the destination folder tree if it doesn't already exist.
//    if (!file_prepare_directory($directory, FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS)) {
//      \Drupal::logger('image')->error('Failed to create style directory: %directory', array('%directory' => $directory));
//      return FALSE;
//    }
//
//    foreach ($this->getEffects() as $effect) {
//      $effect->applyEffect($image);
//    }
//
//    if (!$image->save($derivative_uri)) {
//      if (file_exists($derivative_uri)) {
//        \Drupal::logger('image')->error('Cached image file %destination already exists. There may be an issue with your rewrite configuration.', array('%destination' => $derivative_uri));
//      }
//      return FALSE;
//    }
//
//    return TRUE;
  }


  /**
   * {@inheritdoc}
   */
  public function deleteProcessor(ImageAPIOptimizeProcessorInterface $processor) {
    $this->getProcessors()->removeInstanceId($processor->getUuid());
    $this->save();
    return $this;
  }

  /**
   * {@inheritdoc}
   */
  public function getProcessor($effect) {
    return $this->getProcessors()->get($effect);
  }

  /**
   * {@inheritdoc}
   */
  public function getProcessors() {
    if (!$this->processorsCollection) {
      $this->processorsCollection = new ImageAPIOptimizeProcessorPluginCollection($this->getImageAPIOptimizeProcessorPluginManager(), $this->processors);
      $this->processorsCollection->sort();
    }
    return $this->processorsCollection;
  }

  /**
   * {@inheritdoc}
   */
  public function getPluginCollections() {
    return array('processors' => $this->getProcessors());
  }

  /**
   * {@inheritdoc}
   */
  public function addProcessor(array $configuration) {
    $configuration['uuid'] = $this->uuidGenerator()->generate();
    $this->getProcessors()->addInstanceId($configuration['uuid'], $configuration);
    return $configuration['uuid'];
  }

  /**
   * {@inheritdoc}
   */
  public function getReplacementID() {
    /** @var \Drupal\imageapi_optimize\ImageAPIOptimizePipelineStorageInterface $storage */
    $storage = $this->entityTypeManager()->getStorage($this->getEntityTypeId());
    return $storage->getReplacementId($this->id());
  }

  /**
   * {@inheritdoc}
   */
  public function getName() {
    return $this->get('name');
  }

  /**
   * {@inheritdoc}
   */
  public function setName($name) {
    $this->set('name', $name);
    return $this;
  }

  /**
   * Returns the image effect plugin manager.
   *
   * @return \Drupal\Component\Plugin\PluginManagerInterface
   *   The image effect plugin manager.
   */
  protected function getImageAPIOptimizeProcessorPluginManager() {
    return \Drupal::service('plugin.manager.imageapi_optimize.processor');
  }
}
