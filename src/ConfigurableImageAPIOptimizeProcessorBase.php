<?php

namespace Drupal\imageapi_optimize;

use Drupal\Core\Form\FormStateInterface;

/**
 * Provides a base class for configurable image effects.
 *
 * @see \Drupal\imageapi_optimize\Annotation\ImageEffect
 * @see \Drupal\imageapi_optimize\ConfigurableImageEffectInterface
 * @see \Drupal\imageapi_optimize\ImageEffectInterface
 * @see \Drupal\imageapi_optimize\ImageEffectBase
 * @see \Drupal\imageapi_optimize\ImageEffectManager
 * @see plugin_api
 */
abstract class ConfigurableImageAPIOptimizeProcessorBase extends ImageAPIOptimizeProcessorBase implements ConfigurableImageAPIOptimizeProcessorInterface {

  /**
   * {@inheritdoc}
   */
  public function validateConfigurationForm(array &$form, FormStateInterface $form_state) {
  }

  /**
   * {@inheritdoc}
   */
  public function submitConfigurationForm(array &$form, FormStateInterface $form_state) {
  }

}
