<?php
/**
 * @file
 * ImageAPI Optimize module integration.
 */

/**
 * Implements hook_imageapi_optimize_processor_info().
 */
function imageapi_optimize_kraken_imageapi_optimize_processor_info() {
  $processors = array(
    'kraken' => array(
      'label' => t('Kraken.io'),
      'help' => t('Use the Kraken.io service to optimize the image.'),
      'url' => 'http://kraken.io/',
      'handler' => 'ImageAPIOptimizeProcessorKraken',
    ),
  );
  return $processors;
}
