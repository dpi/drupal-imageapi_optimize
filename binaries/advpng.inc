<?php
/**
 * @file
 * AdvanceCOMP PNG Compression Utility integration.
 */

/**
 * Implements imageapi_optimize_TYPE_NAME_info().
 */
function imageapi_optimize_binaries_advpng_info() {
  return array(
    'title' => t('AdvanceCOMP PNG Compression Utility'),
    'url' => 'http://advancemame.sourceforge.net/doc-advpng.html',
    'description' => t('Leave empty to skip this command. You can download it <a href="!link">here</a> (part of AdvanceCOMP).', array(
      '!link' => 'http://advancemame.sourceforge.net/comp-download.html'
    )),
    'weight' => 2,
  );
}

/**
 * AdvanceCOMP PNG Compression Utility ImageAPI Optimize binary callback.
 */
function imageapi_optimize_binaries_advpng($image, $dst, $cmd) {
  if ($image->info['mime_type'] == 'image/png') {
    exec("$cmd -z4q " . escapeshellarg($dst), $return, $output);
  }
}
