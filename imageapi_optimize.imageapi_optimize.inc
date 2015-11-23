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
    'imageapi_optimize_jpegoptim' => array(
      'label' => t('Jpeg Optim'),
      'help' => t('Use Jpeg Optim to optimise the image.'),
      'handler' => 'ImageAPIOptimizeProcessorJpegOptim',
    ),
    'imageapi_optimize_jpegtran' => array(
      'label' => t('Jpeg Tran'),
      'help' => t('Use Jpeg Tran to optimise the image.'),
      'handler' => 'ImageAPIOptimizeProcessorJpegTran',
    ),
  );

  return $processors;
}
