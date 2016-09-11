<?php

namespace Drupal\imageapi_optimize\Form;

use Drupal\Core\Form\FormStateInterface;
use Drupal\imageapi_optimize\ImageAPIOptimizeProcessorManager;
use Drupal\imageapi_optimize\ImageAPIOptimizePipelineInterface;
use Symfony\Component\DependencyInjection\ContainerInterface;

/**
 * Provides an add form for image effects.
 */
class ImageAPIOptimizeProcessorAddForm extends ImageAPIOptimizeProcessorFormBase {

  /**
   * The image effect manager.
   *
   * @var \Drupal\imageapi_optimize\ImageAPIOptimizeProcessorManager
   */
  protected $processorManager;

  /**
   * Constructs a new ImageAPIOptimizeProcessorAddForm.
   *
   * @param \Drupal\imageapi_optimize\ImageAPIOptimizeProcessorManager $effect_manager
   *   The image effect manager.
   */
  public function __construct(ImageAPIOptimizeProcessorManager $effect_manager) {
    $this->processorManager = $effect_manager;
  }

  /**
   * {@inheritdoc}
   */
  public static function create(ContainerInterface $container) {
    return new static(
      $container->get('plugin.manager.imageapi_optimize.processor')
    );
  }

  /**
   * {@inheritdoc}
   */
  public function buildForm(array $form, FormStateInterface $form_state, ImageAPIOptimizePipelineInterface $imageapi_optimize_pipeline = NULL, $imageapi_optimize_processor = NULL) {
    $form = parent::buildForm($form, $form_state, $imageapi_optimize_pipeline, $imageapi_optimize_processor);

    $form['#title'] = $this->t('Add %label processor', array('%label' => $this->imageAPIOptimizeProcessor->label()));
    $form['actions']['submit']['#value'] = $this->t('Add processor');

    return $form;
  }

  /**
   * {@inheritdoc}
   */
  protected function prepareImageAPIOptimizeProcessor($imageapi_optimize_processor) {
    $imageapi_optimize_processor = $this->processorManager->createInstance($imageapi_optimize_processor);
    // Set the initial weight so this effect comes last.
    $imageapi_optimize_processor->setWeight(count($this->imageAPIOptimizePipeline->getProcessors()));
    return $imageapi_optimize_processor;
  }

}
