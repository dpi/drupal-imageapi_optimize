<?php

/**
 * Admin settings form for imageapi optimize.
 */
function imageapi_optimize_settings($form, $form_state) {
  $info = imageapi_optimize_info(TRUE);

  // Services (3rd party and internal) settings.
  $form['imageapi_optimize_service'] = array(
    '#type' => 'fieldset',
    '#title' => t('ImageAPI Optimize Service'),
  );

  $form['imageapi_optimize_service']['imageapi_optimize_service'] = array(
    '#type' => 'radios',
    '#title' => t('Choose which optimization service to use'),
    '#default_value' => variable_get('imageapi_optimize_service', 'internal'),
    '#options' => array(),
  );

  uasort($info['services'], 'drupal_sort_weight');
  foreach ($info['services'] as $name => $service) {
    // Append this service to the available services.
    $form['imageapi_optimize_service']['imageapi_optimize_service']['#options'][$name] = $service['title'];
    if (isset($service['url'])) {
      $form['imageapi_optimize_service']['imageapi_optimize_service']['#options'][$name] = l($service['title'], $service['url']);
    }

    // Build service form if required.
    if (isset($service['callbacks']['form'])) {
      // Include services file if defined and available.
      if (isset($service['file'])) {
        include_once $service['file'];
      }

      $return = $service['callbacks']['form']($info);
      if (is_array($return)) {
        $form['imageapi_optimize_service']['imageapi_optimize_' . $name] = array(
            '#type' => 'fieldset',
            '#title' => t('@service Settings', array('@service' => $service['title'])),
            '#collapsible' => TRUE,
            '#collapsed' => FALSE,
            '#states' => array(
              'visible' => array(
                ':input[name="imageapi_optimize_service"]' => array('value' => $name),
              ),
            ),
          ) + $return;
      }
    }
  }

  return system_settings_form($form);
}