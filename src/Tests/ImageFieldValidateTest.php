<?php

namespace Drupal\imageapi_optimize\Tests;

/**
 * Tests validation functions such as min/max resolution.
 *
 * @group image
 */
class ImageFieldValidateTest extends \Drupal\Tests\image\Functional\ImageFieldValidateTest {

  public static $modules = ['node', 'image', 'field_ui', 'image_module_test', 'imageapi_optimize'];

}
