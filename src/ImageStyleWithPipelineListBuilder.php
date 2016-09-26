<?php

namespace Drupal\imageapi_optimize;

use Drupal\Core\Entity\EntityInterface;
use Drupal\image\ImageStyleListBuilder;

class ImageStyleWithPipelineListBuilder extends ImageStyleListBuilder {

  /**
   * {@inheritdoc}
   */
  public function buildHeader() {
    $header = parent::buildHeader();
    $header['pipeline'] = $this->t('Image Optimize Pipeline');
    return $header;
  }

  /**
   * {@inheritdoc}
   */
  public function buildRow(EntityInterface $entity) {
    /** @var \Drupal\imageapi_optimize\Entity\ImageStyleWithPipeline $entity */
    $pipelineNames = imageapi_optimize_pipeline_options(FALSE);
    $row = parent::buildRow($entity);
    $row['pipeline'] = isset($pipelineNames[$entity->getPipeline()]) ? $pipelineNames[$entity->getPipeline()] : '';
    return $row;
  }
}
