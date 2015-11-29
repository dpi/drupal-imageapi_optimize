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
      'help' => t('Use the resmush.it service to optimize the image.'),
      'url' => 'http://resmush.it/',
      'handler' => 'ImageAPIOptimizeProcessorReSmushIt',
    ),
    'imageapi_optimize_advdef' => array(
      'label' => t('AdvanceCOMP Deflate Compression Utility'),
      'url' => 'http://advancemame.sourceforge.net/comp-download.html',
      'help' => t('Use AdvanceCOMP Deflate to optimize the image.'),
      'handler' => 'ImageAPIOptimizeProcessorAdvDef',
    ),
    'imageapi_optimize_advpng' => array(
      'label' => t('AdvanceCOMP PNG Compression Utility'),
      'url' => 'http://advancemame.sourceforge.net/comp-download.html',
      'help' => t('Use AdvanceCOMP PNG to optimize the image.'),
      'handler' => 'ImageAPIOptimizeProcessorAdvPng',
    ),
    'imageapi_optimize_jfifremove' => array(
      'label' => t('Jfifremove'),
      'url' => 'https://github.com/kormoc/imgopt/blob/master/jfifremove.c',
      'help' => t('Use Jfifremove to optimize the image.'),
      'handler' => 'ImageAPIOptimizeProcessorJfifRemove',
    ),
    'imageapi_optimize_jpegoptim' => array(
      'label' => t('Jpegoptim'),
      'url' => 'https://github.com/tjko/jpegoptim',
      'help' => t('Use Jpegoptim to optimize the image.'),
      'handler' => 'ImageAPIOptimizeProcessorJpegOptim',
    ),
    'imageapi_optimize_jpegtran' => array(
      'label' => t('JPEGTran'),
      'url' => 'http://ijg.org/',
      'help' => t('Use JPEGTran to optimize the image.'),
      'handler' => 'ImageAPIOptimizeProcessorJpegTran',
    ),
    'imageapi_optimize_optipng' => array(
      'label' => t('OptiPNG: Advanced PNG Optimizer'),
      'url' => 'http://optipng.sourceforge.net/',
      'help' => t('Use OptiPNG to optimize the image.'),
      'handler' => 'ImageAPIOptimizeProcessorOptiPng',
    ),
    'imageapi_optimize_pngcrush' => array(
      'label' => t('Pngcrush'),
      'url' => 'http://pmt.sourceforge.net/pngcrush/',
      'help' => t('Use Pngcrush to optimize the image.'),
      'handler' => 'ImageAPIOptimizeProcessorPngCrush',
    ),
    'imageapi_optimize_pngout' => array(
      'label' => t('PNGOUT'),
      'url' => 'http://advsys.net/ken/utils.htm',
      'help' => t('Use PNGOUT to optimize the image.'),
      'handler' => 'ImageAPIOptimizeProcessorPngOut',
    ),
    'imageapi_optimize_pngquant' => array(
      'label' => t('pngquant'),
      'url' => 'http://pngquant.org/',
      'help' => t('Use pngquant to optimize the image.'),
      'handler' => 'ImageAPIOptimizeProcessorPngQuant',
    ),
  );

  return $processors;
}

/**
 * Implements hook_imageapi_optimize_default_pipelines().
 */
function imageapi_optimize_imageapi_optimize_default_pipelines() {
  $pipelines = array();

  // Exported imageapi optimize pipeline: resmush_it.
  $pipelines['resmush_it'] = array(
    'label' => 'reSmush.it',
    'processors' => array(
      array(
        'name' => 'imageapi_optimize_resmushit',
        'data' => array(),
        'weight' => 0,
      ),
    ),
  );

  // Exported imageapi optimize pipeline: local_binaries.
  $pipelines['local_binaries'] = array(
    'label' => 'Local binaries',
    'processors' => array(
      array(
        'name' => 'imageapi_optimize_jpegoptim',
        'data' => array(),
        'weight' => 0,
      ),
      array(
        'name' => 'imageapi_optimize_jpegtran',
        'data' => array(),
        'weight' => 0,
      ),
      array(
        'name' => 'imageapi_optimize_pngquant',
        'data' => array(),
        'weight' => 0,
      ),
      array(
        'name' => 'imageapi_optimize_jfifremove',
        'data' => array(),
        'weight' => 1,
      ),
      array(
        'name' => 'imageapi_optimize_optipng',
        'data' => array(),
        'weight' => 1,
      ),
      array(
        'name' => 'imageapi_optimize_pngcrush',
        'data' => array(),
        'weight' => 2,
      ),
      array(
        'name' => 'imageapi_optimize_advpng',
        'data' => array(),
        'weight' => 3,
      ),
      array(
        'name' => 'imageapi_optimize_advdef',
        'data' => array(),
        'weight' => 4,
      ),
      array(
        'name' => 'imageapi_optimize_pngout',
        'data' => array(),
        'weight' => 5,
      ),
    ),
  );

  return $pipelines;
}