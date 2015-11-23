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
    'imageapi_optimize_advdef' => array(
      'label' => t('AdvanceCOMP Deflate Compression Utility'),
      'url' => 'http://advancemame.sourceforge.net/comp-download.html',
      'help' => t(''),
      'handler' => 'ImageAPIOptimizeProcessorAdvDef',
    ),
    'imageapi_optimize_advpng' => array(
      'label' => t('AdvanceCOMP PNG Compression Utility'),
      'url' => 'http://advancemame.sourceforge.net/comp-download.html',
      'help' => t(''),
      'handler' => 'ImageAPIOptimizeProcessorAdvPng',
    ),
    'imageapi_optimize_jfifremove' => array(
      'label' => t('Jfifremove'),
      'url' => 'https://github.com/kormoc/imgopt/blob/master/jfifremove.c',
      'help' => t(''),
      'handler' => 'ImageAPIOptimizeProcessorJfifRemove',
    ),
    'imageapi_optimize_jpegoptim' => array(
      'label' => t('Jpegoptim'),
      'url' => 'https://github.com/tjko/jpegoptim',
      'help' => t('Use Jpeg Optim to optimise the image.'),
      'handler' => 'ImageAPIOptimizeProcessorJpegOptim',
    ),
    'imageapi_optimize_jpegtran' => array(
      'label' => t('Jpeg Tran'),
      'url' => 'http://ijg.org/',
      'help' => t('Use Jpeg Tran to optimise the image.'),
      'handler' => 'ImageAPIOptimizeProcessorJpegTran',
    ),
    'imageapi_optimize_optipng' => array(
      'label' => t('OptiPNG: Advanced PNG Optimizer'),
      'url' => 'http://optipng.sourceforge.net/',
      'help' => t('Use Jpeg Tran to optimise the image.'),
      'handler' => 'ImageAPIOptimizeProcessorOptiPng',
    ),
    'imageapi_optimize_pngcrush' => array(
      'label' => t('Pngcrush'),
      'url' => 'http://pmt.sourceforge.net/pngcrush/',
      'help' => t('Use Jpeg Tran to optimise the image.'),
      'handler' => 'ImageAPIOptimizeProcessorPngCrush',
    ),
    'imageapi_optimize_pngout' => array(
      'label' => t('PNGOUT'),
      'url' => 'http://advsys.net/ken/utils.htm',
      'help' => t('Use Jpeg Tran to optimise the image.'),
      'handler' => 'ImageAPIOptimizeProcessorPngOut',
    ),
    'imageapi_optimize_pngquant' => array(
      'label' => t('pngquant'),
      'url' => 'http://pngquant.org/',
      'help' => t('Use Jpeg Tran to optimise the image.'),
      'handler' => 'ImageAPIOptimizeProcessorPngQuant',
    ),
  );

  return $processors;
}
