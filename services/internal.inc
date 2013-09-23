<?php
/**
 * @file
 * Internal ImageAPI Optimize service.
 */

/**
 * Implements imageapi_optimize_TYPE_NAME_info().
 */
function imageapi_optimize_services_internal_info() {
  return array(
    'title' => t('Internal tools (configurable below)'),
    'callbacks' => array(
      'form' => 'imageapi_optimize_services_internal_form',
    ),
    'weight' => -10,
  );
}

/**
 * Internal ImageAPI Optimize form callback.
 */
function imageapi_optimize_services_internal_form($info) {
  $form = array();

  foreach ($info['binaries'] as $name => $binary) {
    $form['imageapi_optimize_' . $name] = array(
      '#type' => 'textfield',
      '#title' => t('Path to !name', array('!name' => $name)),
      '#default_value' => variable_get('imageapi_optimize_' . $name, ''),
      '#element_validate' => array('imageapi_optimize_validate_path'),
    );
    if (isset($binary['description'])) {
      $form['imageapi_optimize_' . $name]['#description'] = $binary['description'];
    }
  }

  return $form;
}

/**
 * Internal ImageAPI Optimize service callback.
 */
function imageapi_optimize_services_internal($image, $dst) {
  $dst = drupal_realpath($dst);

  $info = imageapi_optimize_info();
  foreach ($info['binaries'] as $name => $binary) {
    $cmd = variable_get('imageapi_optimize_' . $name, '');
    if ($cmd) {
      // Include binary file if defined and available.
      if (isset($binary['file'])) {
        include_once $binary['file'];
      }

      $binary['callbacks']['process']($image, $dst, $cmd);
    }
  }

  return TRUE;
}

/**
 * Saves result of a command into file.
 */
function _imageapi_optimize_exec($cmd, $dst) {
  ob_start();
  passthru($cmd);
  $output = ob_get_contents();
  ob_end_clean();

  file_unmanaged_save_data($output, $dst, FILE_EXISTS_REPLACE);
}

/**
 * Validation callback for binary fields.
 */
function imageapi_optimize_validate_path($element) {
  $errors = _imageapi_optimize_check_path($element['#value']);
  if ($errors) {
    form_set_error($element['#parents'][0], implode('<br />', $errors));
    return FALSE;
  }
  return TRUE;
}

