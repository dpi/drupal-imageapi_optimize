<?php

namespace Drupal\imageapi_optimize\Form;

use Drupal\Core\Entity\EntityStorageInterface;
use Drupal\Core\Form\FormStateInterface;
use Drupal\Core\Url;
use Drupal\imageapi_optimize\ConfigurableImageAPIOptimizeProcessorInterface;
use Drupal\imageapi_optimize\ImageAPIOptimizeProcessorManager;
use Symfony\Component\DependencyInjection\ContainerInterface;

/**
 * Controller for image style edit form.
 */
class ImageAPIOptimizePipelineEditForm extends ImageAPIOptimizePipelineFormBase {

  /**
   * The image effect manager service.
   *
   * @var \Drupal\imageapi_optimize\ImageAPIOptimizeProcessorManager
   */
  protected $imageAPIOptimizeProcessorManager;

  /**
   * Constructs an ImageAPIOptimizePipelineEditForm object.
   *
   * @param \Drupal\Core\Entity\EntityStorageInterface $imageapi_optimize_processor_storage
   *   The storage.
   * @param \Drupal\imageapi_optimize\ImageAPIOptimizeProcessorManager $imageapi_optimize_processor_manager
   *   The image effect manager service.
   */
  public function __construct(EntityStorageInterface $imageapi_optimize_processor_storage, ImageAPIOptimizeProcessorManager $imageapi_optimize_processor_manager) {
    parent::__construct($imageapi_optimize_processor_storage);
    $this->imageAPIOptimizeProcessorManager = $imageapi_optimize_processor_manager;
  }

  /**
   * {@inheritdoc}
   */
  public static function create(ContainerInterface $container) {
    return new static(
      $container->get('entity.manager')->getStorage('imageapi_optimize_pipeline'),
      $container->get('plugin.manager.imageapi_optimize.processor')
    );
  }

  /**
   * {@inheritdoc}
   */
  public function form(array $form, FormStateInterface $form_state) {
    $user_input = $form_state->getUserInput();
    $form['#title'] = $this->t('Edit pipeline %name', array('%name' => $this->entity->label()));
    $form['#tree'] = TRUE;
    $form['#attached']['library'][] = 'imageapi_optimize/admin';

    // Show the thumbnail preview.
    $preview_arguments = array('#theme' => 'image_style_preview', '#style' => $this->entity);
    $form['preview'] = array(
      '#type' => 'item',
      '#title' => $this->t('Preview'),
      '#markup' => drupal_render($preview_arguments),
      // Render preview above parent elements.
      '#weight' => -5,
    );

    // Build the list of existing image effects for this image style.
    $form['processors'] = array(
      '#type' => 'table',
      '#header' => array(
        $this->t('Processor'),
        $this->t('Weight'),
        $this->t('Operations'),
      ),
      '#tabledrag' => array(
        array(
          'action' => 'order',
          'relationship' => 'sibling',
          'group' => 'image-processor-order-weight',
        ),
      ),
      '#attributes' => array(
        'id' => 'image-style-effects',
      ),
      '#empty' => t('There are currently no processors in this style. Add one by selecting an option below.'),
      // Render effects below parent elements.
      '#weight' => 5,
    );
    foreach ($this->entity->getProcessors() as $processor) {
      $key = $processor->getUuid();
      $form['processors'][$key]['#attributes']['class'][] = 'draggable';
      $form['processors'][$key]['#weight'] = isset($user_input['processors']) ? $user_input['processors'][$key]['weight'] : NULL;
      $form['processors'][$key]['processor'] = array(
        '#tree' => FALSE,
        'data' => array(
          'label' => array(
            '#plain_text' => $processor->label(),
          ),
        ),
      );

      $summary = $processor->getSummary();

      if (!empty($summary)) {
        $summary['#prefix'] = ' ';
        $form['effects'][$key]['processor']['data']['summary'] = $summary;
      }

      $form['processors'][$key]['weight'] = array(
        '#type' => 'weight',
        '#title' => $this->t('Weight for @title', array('@title' => $processor->label())),
        '#title_display' => 'invisible',
        '#default_value' => $processor->getWeight(),
        '#attributes' => array(
          'class' => array('image-processor-order-weight'),
        ),
      );

      $links = array();
      $is_configurable = $processor instanceof ConfigurableImageAPIOptimizeProcessorInterface;
      if ($is_configurable) {
        $links['edit'] = array(
          'title' => $this->t('Edit'),
          'url' => Url::fromRoute('imageapi_optimize.processor_edit_form', [
            'imageapi_optimize_pipeline' => $this->entity->id(),
            'imageapi_optimize_processor' => $key,
          ]),
        );
      }
      $links['delete'] = array(
        'title' => $this->t('Delete'),
        'url' => Url::fromRoute('imageapi_optimize.processor_delete_form', [
          'imageapi_optimize_pipeline' => $this->entity->id(),
          'imageapi_optimize_processor' => $key,
        ]),
      );
      $form['processors'][$key]['operations'] = array(
        '#type' => 'operations',
        '#links' => $links,
      );
    }

    // Build the new image processor addition form and add it to the processor list.
    $new_effect_options = array();
    $effects = $this->imageAPIOptimizeProcessorManager->getDefinitions();
    uasort($effects, function ($a, $b) {
      return strcasecmp($a['id'], $b['id']);
    });
    foreach ($effects as $processor => $definition) {
      $new_effect_options[$processor] = $definition['label'];
    }
    $form['effects']['new'] = array(
      '#tree' => FALSE,
      '#weight' => isset($user_input['weight']) ? $user_input['weight'] : NULL,
      '#attributes' => array('class' => array('draggable')),
    );
    $form['effects']['new']['processor'] = array(
      'data' => array(
        'new' => array(
          '#type' => 'select',
          '#title' => $this->t('Effect'),
          '#title_display' => 'invisible',
          '#options' => $new_effect_options,
          '#empty_option' => $this->t('Select a new processor'),
        ),
        array(
          'add' => array(
            '#type' => 'submit',
            '#value' => $this->t('Add'),
            '#validate' => array('::effectValidate'),
            '#submit' => array('::submitForm', '::effectSave'),
          ),
        ),
      ),
      '#prefix' => '<div class="image-style-new">',
      '#suffix' => '</div>',
    );

    $form['effects']['new']['weight'] = array(
      '#type' => 'weight',
      '#title' => $this->t('Weight for new processor'),
      '#title_display' => 'invisible',
      '#default_value' => count($this->entity->getEffects()) + 1,
      '#attributes' => array('class' => array('image-processor-order-weight')),
    );
    $form['effects']['new']['operations'] = array(
      'data' => array(),
    );

    return parent::form($form, $form_state);
  }

  /**
   * Validate handler for image effect.
   */
  public function effectValidate($form, FormStateInterface $form_state) {
    if (!$form_state->getValue('new')) {
      $form_state->setErrorByName('new', $this->t('Select an effect to add.'));
    }
  }

  /**
   * Submit handler for image effect.
   */
  public function effectSave($form, FormStateInterface $form_state) {
    $this->save($form, $form_state);

    // Check if this field has any configuration options.
    $effect = $this->imageAPIOptimizeProcessorManager->getDefinition($form_state->getValue('new'));

    // Load the configuration form for this option.
    if (is_subclass_of($effect['class'], '\Drupal\imageapi_optimize\ConfigurableImageAPIOptimizeProcessorInterface')) {
      $form_state->setRedirect(
        'image.effect_add_form',
        array(
          'image_style' => $this->entity->id(),
          'image_effect' => $form_state->getValue('new'),
        ),
        array('query' => array('weight' => $form_state->getValue('weight')))
      );
    }
    // If there's no form, immediately add the image effect.
    else {
      $effect = array(
        'id' => $effect['id'],
        'data' => array(),
        'weight' => $form_state->getValue('weight'),
      );
      $effect_id = $this->entity->addImageAPIOptimizeProcessor($effect);
      $this->entity->save();
      if (!empty($effect_id)) {
        drupal_set_message($this->t('The image effect was successfully applied.'));
      }
    }
  }

  /**
   * {@inheritdoc}
   */
  public function submitForm(array &$form, FormStateInterface $form_state) {

    // Update image effect weights.
    if (!$form_state->isValueEmpty('effects')) {
      $this->updateEffectWeights($form_state->getValue('effects'));
    }

    parent::submitForm($form, $form_state);
  }

  /**
   * {@inheritdoc}
   */
  public function save(array $form, FormStateInterface $form_state) {
    parent::save($form, $form_state);
    drupal_set_message($this->t('Changes to the style have been saved.'));
  }

  /**
   * {@inheritdoc}
   */
  public function actions(array $form, FormStateInterface $form_state) {
    $actions = parent::actions($form, $form_state);
    $actions['submit']['#value'] = $this->t('Update style');

    return $actions;
  }

  /**
   * Updates image effect weights.
   *
   * @param array $effects
   *   Associative array with effects having effect uuid as keys and array
   *   with effect data as values.
   */
  protected function updateEffectWeights(array $effects) {
    foreach ($effects as $uuid => $effect_data) {
      if ($this->entity->getEffects()->has($uuid)) {
        $this->entity->getEffect($uuid)->setWeight($effect_data['weight']);
      }
    }
  }

}
