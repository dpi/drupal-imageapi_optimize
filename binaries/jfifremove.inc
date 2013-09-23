<?php
/**
 * @file
 * Jfifremove integration.
 */

/**
 * Implements imageapi_optimize_TYPE_NAME_info().
 */
function imageapi_optimize_binaries_jfifremove_info() {
  return array(
    'title' => t('Jfifremove'),
    'url' => 'https://github.com/kormoc/imgopt/blob/master/jfifremove.c',
    'description' => t('Leave empty to skip this command. You can download it <a href="!link">here</a>.', array(
      '!link' => 'https://github.com/kormoc/imgopt/blob/master/jfifremove.c'
    )),
    'weight' => 4,
  );
}

/**
 * Jfifremove ImageAPI Optimize binary callback.
 */
function imageapi_optimize_binaries_jfifremove($image, $dst, $cmd) {
  if ($image->info['mime_type'] == 'image/jpeg') {
    _imageapi_optimize_exec("$cmd < " . escapeshellarg($dst), $dst);
  }
}
