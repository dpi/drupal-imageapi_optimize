<?php

namespace Drupal\Tests\imageapi_optimize\Kernel;

/**
 * Tests using entity fields of the image field type.
 *
 * @group image
 */
class ImageItemTest extends \Drupal\Tests\image\Kernel\ImageItemTest {

  /**
   * {@inheritdoc}
   */
  public static $modules = ['file', 'image', 'imageapi_optimize'];

}
