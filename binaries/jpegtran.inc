<?php
/**
 * @file
 * JPEGTran integration.
 */

/**
 * Implements imageapi_optimize_TYPE_NAME_info().
 */
function imageapi_optimize_binaries_jpegtran_info() {
  return array(
    'title' => t('JPEGTran'),
    'url' => 'http://ijg.org/',
    'description' => t('Leave empty to skip this command. This is a part of <a href="!link">libjpeg</a> and could probably on your system.', array(
      '!link' => 'http://ijg.org/'
    )),
    'weight' => 3,
  );
}

/**
 * JPEGTran ImageAPI Optimize binary callback.
 */
function imageapi_optimize_binaries_jpegtran($image, $dst, $cmd) {
  if ($image->info['mime_type'] == 'image/jpeg') {
    _imageapi_optimize_exec("$cmd -copy none -optimize " . escapeshellarg($dst), $dst);
  }
}
