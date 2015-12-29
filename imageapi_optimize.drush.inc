<?php

/**
 * Drush command for ImageAPI Optimize.
 */

/**
 * Implements hook_drush_command().
 */
function imageapi_optimize_drush_command() {
  $commands = array();

  $commands['imageapi-optimize'] = array(
    'description' => dt('Optimize images using a specified pipeline.'),
    'drupal dependencies' => array(
      'imageapi_optimize',
    ),
    'arguments' => array(
      'path' => array(
        dt('The path of a single image or directory of images to optimize.'),
      ),
    ),
    'required-arguments' => TRUE,
    'options' => array(
      'pipeline' => array(
        'description' => dt('Which imageapi optimize pipeline to use.'),
        'required' => TRUE,
      ),
      'recursive' => array(
        'description' => dt('If a folder is specified to optimize, should it be searched recursively for files.'),
      ),
      'extensions' => array(
        'description' => dt('Comma separated list of file extensions to search for. Default: png,jpg,jpeg'),
      ),
    ),
  );

  return $commands;
}

/**
 * Validation for the imageapi optimize Drush command.
 */
function drush_imageapi_optimize_validate() {
  $pipeline = drush_get_option('pipeline');
  $pipelines = array();
  foreach (imageapi_optimize_pipelines() as $pipe) {
    $pipelines[$pipe['name']] = $pipe['label'];
  }
  $pipe = imageapi_optimize_pipeline_load($pipeline);
  if (empty($pipe)) {
    $choice = drush_choice($pipelines, 'Enter a number to choose which pipeline to use.', '!value');
    if ($choice !== FALSE) {
      drush_set_option('pipeline', $choice);
    }
    else {
      return drush_set_error('Could not find pipeline with the specified name.');
    }
  }

  // Validate that we were given some file paths, maybe do the filescan and show the continue form?
}

/**
 * The ImageAPI Optimize Drush command.
 */
function drush_imageapi_optimize() {
  $cwd = drush_cwd();
  // Shell globbing of paths might deliver us lots of directories.
  $paths = func_get_args();

  $recursive = (bool) drush_get_option('recursive', TRUE);
  $pipeline = drush_get_option('pipeline');

  $image_extensions = explode(',', drush_get_option('extensions', 'png,jpg,jpeg'));
  $file_mask = '/\.(' . implode('|', array_map('preg_quote', array_map('trim', $image_extensions))) . ')/i';

  $files = array();

  foreach ($paths as $rpath) {
    $path = realpath($cwd . DIRECTORY_SEPARATOR . $rpath);
    if (is_file($path)) {
      if (preg_match($file_mask, $path)) {
        $filesize = filesize($path);
        $files[$path] = array(
          'size' => $filesize,
        );
      }
    }
    elseif (is_dir($path)) {
      $scan = file_scan_directory($path, $file_mask, array('recurse' => $recursive));
      foreach ($scan as $found) {
        $filesize = filesize($found->uri);
        $files[$found->uri] = array(
          'size' => $filesize,
        );
      }
    }
  }

  if (empty($files)) {
    return drush_set_error('No files found that match the file mask');
  }

  $total_size = 0;
  foreach ($files as $file) {
    $total_size += $file['size'];
  }

  if (!drush_confirm(dt('Are you sure you want to optimize the @count files found (total size !size)', array(
    '@count' => count($files),
    '!size' => format_size($total_size),
  )))
  ) {
    return drush_set_error('Operation aborted.');
  }

  foreach ($files as $file => $info) {
    $batch['operations'][] = array(
      '_drush_imageapi_optimize_batch_optimize_single',
      array($file, $pipeline),
    );
  }

  $batch['finished'] = '_drush_imageapi_optimize_batch_finished';
  batch_set($batch);
  $batch =& batch_get();
  $batch['progressive'] = FALSE;
  drush_backend_batch_process();
}

/**
 * Batch callback for optimizing a single image.
 */
function _drush_imageapi_optimize_batch_optimize_single($file, $pipeline_name, &$context) {
  drush_log(dt('Optimizing file: @file', array('@file' => $file)));
  $filesize_before = filesize($file);
  imageapi_optimize_optimize_file($pipeline_name, $file);
  clearstatcache(TRUE, $file);
  $filesize_after = filesize($file);
  $context['results']['before'] += $filesize_before;
  $context['results']['after'] += $filesize_after;
  drush_log(dt('Optimized file: @file - saving @bytes', array('@file' => $file, '@bytes' => format_size($filesize_before - $filesize_after))), 'ok');
}

/**
 * Batch finished callback for optimizing images.
 */
function _drush_imageapi_optimize_batch_finished($success, $results, $operations) {
  drush_log(dt('Files optimized, savings made: !total_savings - @percentage_savings%', array(
    '!total_savings' => format_size($results['before'] - $results['after']),
    '@percentage_savings' => round(($results['before'] - $results['after']) / $results['before'] * 100, 1),
  )), 'ok');
}
