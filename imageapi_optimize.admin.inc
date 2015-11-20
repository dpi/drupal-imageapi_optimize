<?php

/**
 * Menu callback; Listing of all current image styles.
 */
function imageapi_optimize_pipeline_list() {
  $page = array();

  $pipelines = imageapi_optimize_pipelines();
  $page['imageapi_optimize_pipeline_list'] = array(
    '#markup' => theme('imageapi_optimize_pipeline_list', array('pipelines' => $pipelines)),
    '#attached' => array(
      'css' => array(drupal_get_path('module', 'image') . '/image.admin.css' => array()),
    ),
  );

  return $page;

}

/**
 * Form builder; Edit an image style name and effects order.
 *
 * @param $form_state
 *   An associative array containing the current state of the form.
 * @param $pipeline
 *   An image style array.
 * @ingroup forms
 * @see image_style_form_submit()
 */
function imageapi_optimize_pipeline_form($form, &$form_state, $pipeline) {
  $title = t('Edit %name pipeline', array('%name' => $pipeline['label']));
  drupal_set_title($title, PASS_THROUGH);

  // Adjust this form for styles that must be overridden to edit.
  $editable = (bool) ($pipeline['storage'] & IMAGEAPI_OPTIMIZE_STORAGE_EDITABLE);

  if (!$editable && empty($form_state['input'])) {
    drupal_set_message(t('This image pipeline is currently being provided by a module. Click the "Override defaults" button to change its settings.'), 'warning');
  }

  $form_state['pipeline'] = $pipeline;
  $form['#tree'] = TRUE;
  $form['#attached']['css'][drupal_get_path('module', 'image') . '/image.admin.css'] = array();

  // Show the Image Style label.
  $form['label'] = array(
    '#type' => 'textfield',
    '#title' => t('Imageapi optimize pipeline name'),
    '#default_value' => $pipeline['label'],
    '#disabled' => !$editable,
    '#required' => TRUE,
  );

  // Allow the name of the style to be changed, unless this style is
  // provided by a module's hook_default_image_styles().
  $form['name'] = array(
    '#type' => 'machine_name',
    '#size' => '64',
    '#default_value' => $pipeline['name'],
    '#disabled' => !$editable,
    '#description' => t('Use only lowercase alphanumeric characters, underscores (_), and hyphens (-).'),
    '#required' => TRUE,
    '#machine_name' => array(
      'exists' => 'imageapi_optimize_pipeline_load',
      'source' => array('label'),
      'replace_pattern' => '[^0-9a-z_\-]',
      'error' => t('Please only use lowercase alphanumeric characters, underscores (_), and hyphens (-) for style names.'),
    ),
  );

  // Build the list of existing image effects for this image style.
  $form['processors'] = array(
    '#theme' => 'imageapi_optimize_pipeline_processors',
  );
  foreach ($pipeline['processors'] as $key => $processor) {
    $form['processors'][$key]['#weight'] = isset($form_state['input']['processors']) ? $form_state['input']['processors'][$key]['weight'] : NULL;
    $form['processors'][$key]['label'] = array(
      '#markup' => $processor['label'],
    );
    $form['processors'][$key]['summary'] = array(
      '#markup' => isset($processor['summary theme']) ? theme($processor['summary theme'], array('data' => $processor['data'])) : '',
    );
    $form['processors'][$key]['weight'] = array(
      '#type' => 'weight',
      '#title' => t('Weight for @title', array('@title' => $processor['label'])),
      '#title_display' => 'invisible',
      '#default_value' => $processor['weight'],
      '#access' => $editable,
    );

    // Only attempt to display these fields for editable styles as the 'ieid'
    // key is not set for styles defined in code.
    if ($editable) {
      $form['processors'][$key]['configure'] = array(
        '#type' => 'link',
        '#title' => t('edit'),
        '#href' => 'admin/config/media/imageapi-optimize/edit/' . $pipeline['name'] . '/processors/' . $processor['ieid'],
        '#access' => $editable && isset($processor['form callback']),
      );
      $form['processors'][$key]['remove'] = array(
        '#type' => 'link',
        '#title' => t('delete'),
        '#href' => 'admin/config/media/imageapi-optimize/edit/' . $pipeline['name'] . '/processors/' . $processor['ieid'] . '/delete',
        '#access' => $editable,
      );
    }
  }

  // Build the new image effect addition form and add it to the effect list.
  $new_processor_options = array();
  foreach (imageapi_optimize_processor_definitions() as $processor => $definition) {
    $new_processor_options[$processor] = check_plain($definition['label']);
  }
  $form['processors']['new'] = array(
    '#tree' => FALSE,
    '#weight' => isset($form_state['input']['weight']) ? $form_state['input']['weight'] : NULL,
    '#access' => $editable,
  );
  $form['processors']['new']['new'] = array(
    '#type' => 'select',
    '#title' => t('Processor'),
    '#title_display' => 'invisible',
    '#options' => $new_processor_options,
    '#empty_option' => t('Select a new processor'),
  );
  $form['processors']['new']['weight'] = array(
    '#type' => 'weight',
    '#title' => t('Weight for new processor'),
    '#title_display' => 'invisible',
    '#default_value' => count($form['processors']) - 1,
  );
  $form['processors']['new']['add'] = array(
    '#type' => 'submit',
    '#value' => t('Add'),
    '#validate' => array('imageapi_optimize_pipeline_form_add_validate'),
    '#submit' => array('imageapi_optimize_pipeline_form_submit', 'imageapi_optimize_pipeline_form_add_submit'),
  );

  // Show the Override or Submit button for this style.
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['override'] = array(
    '#type' => 'submit',
    '#value' => t('Override defaults'),
    '#validate' => array(),
    '#submit' => array('imageapi_optimize_pipeline_form_override_submit'),
    '#access' => !$editable,
  );
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update pipeline'),
    '#access' => $editable,
  );

  return $form;
}

/**
 * Validate handler for adding a new image effect to an image style.
 */
function imageapi_optimize_pipeline_form_add_validate($form, &$form_state) {
  if (!$form_state['values']['new']) {
    form_error($form['processors']['new']['new'], t('Select a processor to add.'));
  }
}

/**
 * Submit handler for adding a new image effect to an image style.
 */
function imageapi_optimize_pipeline_form_add_submit($form, &$form_state) {
  $pipeline = $form_state['pipeline'];
  // Check if this field has any configuration options.
  $processor = imageapi_optimize_processor_definition_load($form_state['values']['new']);

  // Load the configuration form for this option.
  if (isset($processor['form callback'])) {
    $path = 'admin/config/media/imageapi-optimize/edit/' . $form_state['pipeline']['name'] . '/add/' . $form_state['values']['new'];
    $form_state['redirect'] = array($path, array('query' => array('weight' => $form_state['values']['weight'])));
  }
  // If there's no form, immediately add the image effect.
  else {
    $processor['isid'] = $pipeline['isid'];
    $processor['weight'] = $form_state['values']['weight'];
    imageapi_optimize_processor_save($processor);
    drupal_set_message(t('The imageapi optimize processor was successfully applied.'));
  }
}

/**
 * Submit handler for overriding a module-defined style.
 */
function imageapi_optimize_pipeline_form_override_submit($form, &$form_state) {
  drupal_set_message(t('The %pipeline pipeline has been overridden, allowing you to change its settings.', array('%pipeline' => $form_state['pipeline']['label'])));
  imageapi_optimize_default_pipeline_save($form_state['pipeline']);
}

/**
 * Submit handler for saving an image style.
 */
function imageapi_optimize_pipeline_form_submit($form, &$form_state) {
  // Update the image style.
  $pipeline = $form_state['pipeline'];
  $pipeline['name'] = $form_state['values']['name'];
  $pipeline['label'] = $form_state['values']['label'];

  // Update image effect weights.
  if (!empty($form_state['values']['processors'])) {
    foreach ($form_state['values']['processors'] as $ieid => $processor_data) {
      if (isset($pipeline['processors'][$ieid])) {
        $processor = $pipeline['processors'][$ieid];
        $processor['weight'] = $processor_data['weight'];
        imageapi_optimize_processor_save($processor);
      }
    }
  }

  imageapi_optimize_pipeline_save($pipeline);
  if ($form_state['values']['op'] == t('Update pipeline')) {
    drupal_set_message(t('Changes to the pipeline have been saved.'));
  }
  $form_state['redirect'] = 'admin/config/media/imageapi-optimize/edit/' . $pipeline['name'];
}

/**
 * Form builder; Form for adding a new image style.
 *
 * @ingroup forms
 * @see image_style_add_form_submit()
 */
function imageapi_optimize_pipeline_add_form($form, &$form_state) {
  $form['label'] = array(
    '#type' => 'textfield',
    '#title' => t('Pipeline name'),
    '#default_value' => '',
    '#required' => TRUE,
  );
  $form['name'] = array(
    '#type' => 'machine_name',
    '#description' => t('Use only lowercase alphanumeric characters, underscores (_), and hyphens (-).'),
    '#size' => '64',
    '#required' => TRUE,
    '#machine_name' => array(
      'exists' => 'imageapi_optimize_pipeline_load',
      'source' => array('label'),
      'replace_pattern' => '[^0-9a-z_\-]',
      'error' => t('Please only use lowercase alphanumeric characters, underscores (_), and hyphens (-) for style names.'),
    ),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Create new pipeline'),
  );

  return $form;
}

/**
 * Submit handler for adding a new image style.
 */
function imageapi_optimize_pipeline_add_form_submit($form, &$form_state) {
  $style = array(
    'name' => $form_state['values']['name'],
    'label' => $form_state['values']['label'],
  );
  $style = imageapi_optimize_pipeline_save($style);
  drupal_set_message(t('Pipeline %name was created.', array('%name' => $style['label'])));
  $form_state['redirect'] = 'admin/config/media/imageapi-optimize/edit/' . $style['name'];
}

/**
 * Element validate function to ensure unique, URL safe style names.
 *
 * This function is no longer used in Drupal core since image style names are
 * now validated using #machine_name functionality. It is kept for backwards
 * compatibility (since non-core modules may be using it) and will be removed
 * in Drupal 8.
 */
function imageapi_optimize_pipeline_name_validate($element, $form_state) {
  // Check for duplicates.
  $pipelines = imageapi_optimize_pipelines();
  if (isset($pipelines[$element['#value']]) && (!isset($form_state['pipeline']['isid']) || $pipelines[$element['#value']]['isid'] != $form_state['pipeline']['isid'])) {
    form_set_error($element['#name'], t('The imageapi optimize pipeline name %name is already in use.', array('%name' => $element['#value'])));
  }

  // Check for illegal characters in image style names.
  if (preg_match('/[^0-9a-z_\-]/', $element['#value'])) {
    form_set_error($element['#name'], t('Please only use lowercase alphanumeric characters, underscores (_), and hyphens (-) for pipeline names.'));
  }
}

/**
 * Form builder; Form for deleting an image style.
 *
 * @param $pipeline
 *   An image style array.
 *
 * @ingroup forms
 * @see image_style_delete_form_submit()
 */
function imageapi_optimize_pipeline_delete_form($form, &$form_state, $pipeline) {
  $form_state['pipeline'] = $pipeline;

  $replacement_pipeline = array_diff_key(imageapi_optimize_pipeline_options(TRUE, PASS_THROUGH), array($pipeline['name'] => ''));
  $form['replacement'] = array(
    '#title' => t('Replacement pipeline'),
    '#type' => 'select',
    '#options' => $replacement_pipeline,
    '#empty_option' => t('No replacement, just delete'),
  );

  return confirm_form(
    $form,
    t('Optionally select a pipeline before deleting %pipeline', array('%pipeline' => $pipeline['label'])),
    'admin/config/media/imageapi-optimize',
    t('If this pipeline is in use on the site, you may select another pipeline to replace it.'),
    t('Delete'),  t('Cancel')
  );
}

/**
 * Submit handler to delete an image style.
 */
function imageapi_optimize_pipeline_delete_form_submit($form, &$form_state) {
  $pipeline = $form_state['pipeline'];

  imageapi_optimize_pipeline_delete($pipeline, $form_state['values']['replacement']);
  drupal_set_message(t('Pipeline %name was deleted.', array('%name' => $pipeline['label'])));
  $form_state['redirect'] = 'admin/config/media/imageapi-optimize';
}

/**
 * Confirmation form to revert a database style to its default.
 */
function imageapi_optimize_pipeline_revert_form($form, &$form_state, $pipeline) {
  $form_state['pipeline'] = $pipeline;

  return confirm_form(
    $form,
    t('Revert the %pipeline pipeline?', array('%pipeline' => $pipeline['label'])),
    'admin/config/media/imageapi-optimize',
    t('Reverting this pipeline will delete the customized settings and restore the defaults provided by the @module module.', array('@module' => $pipeline['module'])),
    t('Revert'),  t('Cancel')
  );
}

/**
 * Submit handler to convert an overridden style to its default.
 */
function imageapi_optimize_pipeline_revert_form_submit($form, &$form_state) {
  drupal_set_message(t('The %pipeline pipeline has been reverted to its defaults.', array('%pipeline' => $form_state['pipeline']['label'])));
  imageapi_optimize_default_pipeline_revert($form_state['pipeline']);
  $form_state['redirect'] = 'admin/config/media/imageapi-optimize';
}

/**
 * Form builder; Form for adding and editing image effects.
 *
 * This form is used universally for editing all image effects. Each effect adds
 * its own custom section to the form by calling the form function specified in
 * hook_image_effects().
 *
 * @param $form_state
 *   An associative array containing the current state of the form.
 * @param $pipeline
 *   An image style array.
 * @param $processor
 *   An image effect array.
 *
 * @ingroup forms
 * @see hook_image_effects()
 * @see image_effects()
 * @see image_resize_form()
 * @see image_scale_form()
 * @see image_rotate_form()
 * @see image_crop_form()
 * @see image_effect_form_submit()
 */
function imageapi_optimize_processor_form($form, &$form_state, $pipeline, $processor) {
  if (!empty($processor['data'])) {
    $title = t('Edit %label processor', array('%label' => $processor['label']));
  }
  else{
    $title = t('Add %label processor', array('%label' => $processor['label']));
  }
  drupal_set_title($title, PASS_THROUGH);

  $form_state['pipeline'] = $pipeline;
  $form_state['processor'] = $processor;

  // If no configuration for this image effect, return to the image style page.
  if (!isset($processor['form callback'])) {
    drupal_goto('admin/config/media/imageapi-optimize/edit/' . $pipeline['name']);
  }

  $form['#tree'] = TRUE;
  $form['#attached']['css'][drupal_get_path('module', 'image') . '/image.admin.css'] = array();
  if (function_exists($processor['form callback'])) {
    $form['data'] = call_user_func($processor['form callback'], $processor['data']);
  }

  // Check the URL for a weight, then the image effect, otherwise use default.
  $form['weight'] = array(
    '#type' => 'hidden',
    '#value' => isset($_GET['weight']) ? intval($_GET['weight']) : (isset($processor['weight']) ? $processor['weight'] : count($pipeline['effects'])),
  );

  $form['actions'] = array('#tree' => FALSE, '#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => isset($processor['ieid']) ? t('Update processor') : t('Add processor'),
  );
  $form['actions']['cancel'] = array(
    '#type' => 'link',
    '#title' => t('Cancel'),
    '#href' => 'admin/config/media/imageapi-optimize/edit/' . $pipeline['name'],
  );

  return $form;
}

/**
 * Submit handler for updating an image effect.
 */
function imageapi_optimize_processor_form_submit($form, &$form_state) {
  $pipeline = $form_state['pipeline'];
  $processor = array_merge($form_state['processor'], $form_state['values']);
  $processor['isid'] = $pipeline['isid'];
  imageapi_optimize_processor_save($processor);
  drupal_set_message(t('The image processor was successfully applied.'));
  $form_state['redirect'] = 'admin/config/media/imageapi-optimize/edit/' . $pipeline['name'];
}

/**
 * Form builder; Form for deleting an image effect.
 *
 * @param $pipeline
 *   Name of the image style from which the image effect will be removed.
 * @param $processor
 *   Name of the image effect to remove.
 * @ingroup forms
 * @see image_effect_delete_form_submit()
 */
function imageapi_optimize_processor_delete_form($form, &$form_state, $pipeline, $processor) {
  $form_state['pipeline'] = $pipeline;
  $form_state['processor'] = $processor;

  $question = t('Are you sure you want to delete the @processor processor from the %pipeline pipeline?', array('%pipeline' => $pipeline['label'], '@processor' => $processor['label']));
  return confirm_form($form, $question, 'admin/config/media/imageapi-optimize/edit/' . $pipeline['name'], '', t('Delete'));
}

/**
 * Submit handler to delete an image effect.
 */
function imageapi_optimize_processor_delete_form_submit($form, &$form_state) {
  $pipeline = $form_state['pipeline'];
  $processor = $form_state['processor'];

  imageapi_optimize_processor_delete($processor);
  drupal_set_message(t('The processor %name has been deleted.', array('%name' => $processor['label'])));
  $form_state['redirect'] = 'admin/config/media/imageapi-optimize/edit/' . $pipeline['name'];
}

/**
 * Returns HTML for the page containing the list of image styles.
 *
 * @param $variables
 *   An associative array containing:
 *   - styles: An array of all the image styles returned by image_get_styles().
 *
 * @see image_get_styles()
 * @ingroup themeable
 */
function theme_imageapi_optimize_pipeline_list($variables) {
  $pipelines = $variables['pipelines'];

  $header = array(t('Pipeline name'), t('Settings'), array('data' => t('Operations'), 'colspan' => 3));
  $rows = array();
  foreach ($pipelines as $pipeline) {
    $row = array();
    $row[] = l($pipeline['label'], 'admin/config/media/imageapi-optimize/edit/' . $pipeline['name']);
    $link_attributes = array(
      'attributes' => array(
        'class' => array('image-style-link'),
      ),
    );
    if ($pipeline['storage'] == IMAGEAPI_OPTIMIZE_STORAGE_NORMAL) {
      $row[] = t('Custom');
      $row[] = l(t('edit'), 'admin/config/media/imageapi-optimize/edit/' . $pipeline['name'], $link_attributes);
      $row[] = l(t('delete'), 'admin/config/media/imageapi-optimize/delete/' . $pipeline['name'], $link_attributes);
    }
    elseif ($pipeline['storage'] == IMAGEAPI_OPTIMIZE_STORAGE_OVERRIDE) {
      $row[] = t('Overridden');
      $row[] = l(t('edit'), 'admin/config/media/imageapi-optimize/edit/' . $pipeline['name'], $link_attributes);
      $row[] = l(t('revert'), 'admin/config/media/imageapi-optimize/revert/' . $pipeline['name'], $link_attributes);
    }
    else {
      $row[] = t('Default');
      $row[] = l(t('edit'), 'admin/config/media/imageapi-optimize/edit/' . $pipeline['name'], $link_attributes);
      $row[] = '';
    }
    $rows[] = $row;
  }

  if (empty($rows)) {
    $rows[] = array(array(
      'colspan' => 4,
      'data' => t('There are currently no pipelines. <a href="!url">Add a new one</a>.', array('!url' => url('admin/config/media/imageapi-optimize/add'))),
    ));
  }

  return theme('table', array('header' => $header, 'rows' => $rows));
}

/**
 * Returns HTML for a listing of the effects within a specific image style.
 *
 * @param $variables
 *   An associative array containing:
 *   - form: A render element representing the form.
 *
 * @ingroup themeable
 */
function theme_imageapi_optimize_pipeline_processors($variables) {
  $form = $variables['form'];

  $rows = array();

  foreach (element_children($form) as $key) {
    $row = array();
    $form[$key]['weight']['#attributes']['class'] = array('image-effect-order-weight');
    if (is_numeric($key)) {
      $summary = drupal_render($form[$key]['summary']);
      $row[] = drupal_render($form[$key]['label']) . (empty($summary) ? '' : ' ' . $summary);
      $row[] = drupal_render($form[$key]['weight']);
      $row[] = drupal_render($form[$key]['configure']);
      $row[] = drupal_render($form[$key]['remove']);
    }
    else {
      // Add the row for adding a new image effect.
      $row[] = '<div class="image-style-new">' . drupal_render($form['new']['new']) . drupal_render($form['new']['add']) . '</div>';
      $row[] = drupal_render($form['new']['weight']);
      $row[] = array('data' => '', 'colspan' => 2);
    }

    if (!isset($form[$key]['#access']) || $form[$key]['#access']) {
      $rows[] = array(
        'data' => $row,
        'class' => !empty($form[$key]['weight']['#access']) || $key == 'new' ? array('draggable') : array(),
      );
    }
  }

  $header = array(
    t('Processor'),
    t('Weight'),
    array('data' => t('Operations'), 'colspan' => 2),
  );

  if (count($rows) == 1 && $form['new']['#access']) {
    array_unshift($rows, array(array(
      'data' => t('There are currently no processors in this pipeline. Add one by selecting an option below.'),
      'colspan' => 4,
    )));
  }

  $output = theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('id' => 'image-style-effects')));
  drupal_add_tabledrag('image-style-effects', 'order', 'sibling', 'image-effect-order-weight');
  return $output;
}
