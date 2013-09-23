<?php
/**
 * @file
 * OptiPNG: Advanced PNG Optimizer integration.
 */

/**
 * Implements imageapi_optimize_TYPE_NAME_info().
 */
function imageapi_optimize_binaries_optipng_info() {
  return array(
    'title' => t('OptiPNG: Advanced PNG Optimizer'),
    'url' => 'http://optipng.sourceforge.net/',
    'description' => t('Leave empty to skip this command. You can download it <a href="!link">here</a>.', array(
      '!link' => 'http://optipng.sourceforge.net/',
    )),
    'weight' => 0,
  );
}

/**
 * OptiPNG: Advanced PNG Optimizer ImageAPI Optimize binary callback.
 */
function imageapi_optimize_binaries_optipng($image, $dst, $cmd) {
  if ($image->info['mime_type'] == 'image/png') {
    exec("$cmd -o7 -quiet " . escapeshellarg($dst));
  }
}
