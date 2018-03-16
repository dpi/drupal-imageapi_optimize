<?php

namespace Drupal\imageapi_optimize\Tests;

/**
 * Tests image style deletion using the UI.
 *
 * @group image
 */
class ImageStyleDeleteTest extends \Drupal\Tests\image\Functional\ImageStyleDeleteTest {

  public static $modules = ['node', 'image', 'field_ui', 'image_module_test', 'imageapi_optimize'];

}
