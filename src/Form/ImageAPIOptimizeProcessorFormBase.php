<?php

namespace Drupal\imageapi_optimize\Form;

use Drupal\Core\Form\FormBase;
use Drupal\Core\Form\FormStateInterface;
use Drupal\Core\Form\SubformState;
use Drupal\imageapi_optimize\ConfigurableImageAPIOptimizeProcessorInterface;
use Drupal\imageapi_optimize\ImageAPIOptimizePipelineInterface;
use Drupal\Component\Plugin\Exception\PluginNotFoundException;
use Symfony\Component\HttpKernel\Exception\NotFoundHttpException;

/**
 * Provides a base form for image effects.
 */
abstract class ImageAPIOptimizeProcessorFormBase extends FormBase {

  /**
   * The image style.
   *
   * @var \Drupal\imageapi_optimize\ImageAPIOptimizePipelineInterface
   */
  protected $imageAPIOptimizePipeline;

  /**
   * The image effect.
   *
   * @var \Drupal\imageapi_optimize\ImageAPIOptimizeProcessorInterface|\Drupal\imageapi_optimize\ConfigurableImageAPIOptimizeProcessorInterface
   */
  protected $imageAPIOptimizeProcessor;

  /**
   * {@inheritdoc}
   */
  public function getFormId() {
    return 'imageapi_optimize_processor_form';
  }

  /**
   * {@inheritdoc}
   *
   * @param \Drupal\imageapi_optimize\ImageAPIOptimizePipelineInterface $imageapi_optimize_pipeline
   *   The image style.
   * @param string $imageapi_optimize_processor
   *   The image effect ID.
   *
   * @return array
   *   The form structure.
   *
   * @throws \Symfony\Component\HttpKernel\Exception\NotFoundHttpException
   */
  public function buildForm(array $form, FormStateInterface $form_state, ImageAPIOptimizePipelineInterface $imageapi_optimize_pipeline = NULL, $imageapi_optimize_processor = NULL) {
    $this->imageAPIOptimizePipeline = $imageapi_optimize_pipeline;
    try {
      $this->imageAPIOptimizeProcessor = $this->prepareImageAPIOptimizeProcessor($imageapi_optimize_processor);
    }
    catch (PluginNotFoundException $e) {
      throw new NotFoundHttpException("Invalid processor id: '$imageapi_optimize_processor'.");
    }
    $request = $this->getRequest();

    if (!($this->imageAPIOptimizeProcessor instanceof ConfigurableImageAPIOptimizeProcessorInterface)) {
      throw new NotFoundHttpException();
    }

    $form['#attached']['library'][] = 'imageapi_optimize/admin';
    $form['uuid'] = array(
      '#type' => 'value',
      '#value' => $this->imageAPIOptimizeProcessor->getUuid(),
    );
    $form['id'] = array(
      '#type' => 'value',
      '#value' => $this->imageAPIOptimizeProcessor->getPluginId(),
    );

    $form['data'] = [];
    $subform_state = SubformState::createForSubform($form['data'], $form, $form_state);
    $form['data'] = $this->imageAPIOptimizeProcessor->buildConfigurationForm($form['data'], $subform_state);
    $form['data']['#tree'] = TRUE;

    // Check the URL for a weight, then the image effect, otherwise use default.
    $form['weight'] = array(
      '#type' => 'hidden',
      '#value' => $request->query->has('weight') ? (int) $request->query->get('weight') : $this->imageAPIOptimizeProcessor->getWeight(),
    );

    $form['actions'] = array('#type' => 'actions');
    $form['actions']['submit'] = array(
      '#type' => 'submit',
      '#button_type' => 'primary',
    );
    $form['actions']['cancel'] = array(
      '#type' => 'link',
      '#title' => $this->t('Cancel'),
      '#url' => $this->imageAPIOptimizePipeline->urlInfo('edit-form'),
      '#attributes' => ['class' => ['button']],
    );
    return $form;
  }

  /**
   * {@inheritdoc}
   */
  public function validateForm(array &$form, FormStateInterface $form_state) {
    // The image effect configuration is stored in the 'data' key in the form,
    // pass that through for validation.
    $this->imageAPIOptimizeProcessor->validateConfigurationForm($form['data'], SubformState::createForSubform($form['data'], $form, $form_state));
  }

  /**
   * {@inheritdoc}
   */
  public function submitForm(array &$form, FormStateInterface $form_state) {
    $form_state->cleanValues();

    // The image effect configuration is stored in the 'data' key in the form,
    // pass that through for submission.
    $this->imageAPIOptimizeProcessor->submitConfigurationForm($form['data'], SubformState::createForSubform($form['data'], $form, $form_state));

    $this->imageAPIOptimizeProcessor->setWeight($form_state->getValue('weight'));
    if (!$this->imageAPIOptimizeProcessor->getUuid()) {
      $this->imageAPIOptimizePipeline->addProcessor($this->imageAPIOptimizeProcessor->getConfiguration());
    }
    $this->imageAPIOptimizePipeline->save();

    drupal_set_message($this->t('The imageapi optimize processor was successfully applied.'));
    $form_state->setRedirectUrl($this->imageAPIOptimizePipeline->urlInfo('edit-form'));
  }

  /**
   * Converts an image effect ID into an object.
   *
   * @param string $imageapi_optimize_processor
   *   The image effect ID.
   *
   * @return \Drupal\imageapi_optimize\ImageAPIOptimizeProcessorInterface
   *   The image effect object.
   */
  abstract protected function prepareImageAPIOptimizeProcessor($imageapi_optimize_processor);

}