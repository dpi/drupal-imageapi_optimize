<?php

namespace Drupal\imageapi_optimize;

use Drupal\Core\Config\Entity\ConfigEntityListBuilder;
use Drupal\Core\Entity\EntityInterface;
use Drupal\Core\Url;

/**
 * Defines a class to build a listing of image style entities.
 *
 * @see \Drupal\imageapi_optimize\Entity\ImageStyle
 */
class ImageAPIOptimizePipelineListBuilder extends ConfigEntityListBuilder {

  /**
   * {@inheritdoc}
   */
  public function buildHeader() {
    $header['label'] = $this->t('Pipeline name');
    return $header + parent::buildHeader();
  }

  /**
   * {@inheritdoc}
   */
  public function buildRow(EntityInterface $entity) {
    $row['label'] = $entity->label();
    return $row + parent::buildRow($entity);
  }

  /**
   * {@inheritdoc}
   */
  public function getDefaultOperations(EntityInterface $entity) {
    $flush = array(
      'title' => t('Flush'),
      'weight' => 200,
      'url' => $entity->urlInfo('flush-form'),
    );

    return parent::getDefaultOperations($entity) + array(
      'flush' => $flush,
    );
  }

  /**
   * {@inheritdoc}
   */
  public function render() {
    $build = parent::render();
    $build['table']['#empty'] = $this->t('There are currently no pipelines. <a href=":url">Add a new one</a>.', [
      ':url' => Url::fromRoute('imageapi_optimize.pipeline_add')->toString(),
    ]);
    $build['config_form'] = \Drupal::formBuilder()->getForm('Drupal\imageapi_optimize\Form\ImageAPIOptimizeDefaultPipelineConfigForm');
    return $build;
  }

}
