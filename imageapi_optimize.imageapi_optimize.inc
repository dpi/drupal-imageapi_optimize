<?php
/**
 * @file
 * ImageAPI Optimize module integration.
 */

/**
 * Implements hook_imageapi_optimize_processor_info().
 */
function imageapi_optimize_imageapi_optimize_processor_info() {
  $processors = array(
    'imageapi_optimize_resmushit' => array(
      'label' => t('Resmush.it'),
      'help' => t('Use the resmush.it service to optimize image.'),
      'handler' => 'ImageAPIOptimizeProcessorReSmushIt',
    ),
  );

  return $processors;
}
