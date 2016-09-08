<?php

/**
 * @file
 * Features integration for the ImageAPI Optimize module.
 */

/**
 * Implements hook_features_api().
 */
function imageapi_optimize_features_api() {
  return array(
    'imageapi_optimize' => array(
      'name' => t('ImageAPI Optimize pipelines'),
      'feature_source' => TRUE,
      'default_hook' => 'imageapi_optimize_default_pipelines',
      'alter_hook' => 'imageapi_optimize_pipelines',
    )
  );
}

/**
 * Implements hook_features_export_options().
 */
function imageapi_optimize_features_export_options() {
  $options = array();
  foreach (imageapi_optimize_pipelines() as $name => $pipeline) {
    $options[$name] = $pipeline['name'];
  }
  return $options;
}

/**
 * Implements hook_features_export().
 */
function imageapi_optimize_features_export($data, &$export, $module_name = '') {
  $pipe = array();
  $map = features_get_default_map('imageapi_optimize');
  foreach ($data as $pipeline) {
    $export['dependencies']['imageapi_optimize'] = 'imageapi_optimize';
    // If another module provides this style, add it as a dependency
    if (isset($map[$pipeline]) && $map[$pipeline] != $module_name) {
      $module = $map[$pipeline];
      $export['dependencies'][$module] = $module;
    }
    // Otherwise, export the style
    elseif (imageapi_optimize_pipeline_load($pipeline)) {
      $export['features']['imageapi_optimize'][$pipeline] = $pipeline;
    }
  }
  return $pipe;
}

/**
 * Implements hook_features_export_render().
 */
function imageapi_optimize_features_export_render($module_name, $data, $export = NULL) {
  $code = array();
  $code[] = '  $pipelines = array();';
  $code[] = '';
  foreach ($data as $name) {
    if ($piepline = imageapi_optimize_pipeline_load($name)) {
      _imageapi_optimize_features_pipeline_sanitize($piepline);
      $pipeline = features_var_export($piepline, '  ');
      $pipeline_identifier = features_var_export($name);
      $code[] = "  // Exported imageapi optimize pipeline: {$name}.";
      $code[] = "  \$pipelines[{$pipeline_identifier}] = {$pipeline};";
      $code[] = "";
    }
  }
  $code[] = '  return $pipelines;';
  $code = implode("\n", $code);
  return array('imageapi_optimize_default_pipelines' => $code);
}

/**
 * Implements hook_features_revert().
 */
function imageapi_optimize_features_revert($module) {
  if ($default_pipelines = features_get_default('imageapi_optimize', $module)) {
    foreach (array_keys($default_pipelines) as $default_pipeline) {
      if ($pipeline = imageapi_optimize_pipeline_load($default_pipeline)) {
        if ($pipeline['storage'] != IMAGEAPI_OPTIMIZE_STORAGE_DEFAULT) {
          imageapi_optimize_default_pipeline_revert($pipeline);
        }
      }
    }
  }
}

/**
 * Remove unnecessary keys for export.
 */
function _imageapi_optimize_features_pipeline_sanitize(array &$pipeline) {
  // Sanitize style: Don't export numeric IDs and things which get overwritten
  // in imageapi_optimize_processors() or are code/storage specific. The name property will be
  // the key of the exported $pipeline array.
  $pipeline = array_diff_key($pipeline, array_flip(array(
    'isid',
    'name',
    'module',
    'storage',
  )));

  // Sanitize processors: all that needs to be kept is name, weight and data,
  // which holds all the processor-specific configuratison. Other keys are assumed
  // to belong to the definition of the processor itself, so not configuration.
  foreach ($pipeline['processors'] as $id => $processor) {
    $pipeline['processors'][$id] = array_intersect_key($processor, array_flip(array(
      'name',
      'data',
      'weight',
    )));
  }
}
