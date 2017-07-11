<?php

namespace Drupal\Tests\imageapi_optimize\Unit;

use Drupal\imageapi_optimize\Plugin\ImageAPIOptimizeProcessor\AdvDef;
use Drupal\Tests\UnitTestCase;

/**
 * Tests AdvDef image optimize plugin.
 *
 * @group imageapi_optimize
 */
class AdvDefTest extends UnitTestCase {
  function testCase() {
    $this->assertTrue(TRUE);

    $advdefMock = $this->getMockBuilder('\Drupal\imageapi_optimize\Plugin\ImageAPIOptimizeProcessor\AdvDef')
      ->setMethods(['getFullPathToBinary', 'execShellCommand', 'getMimeType', 'sanitizeFilename'])
      ->disableOriginalConstructor()
      ->disableOriginalClone()
      ->disableArgumentCloning()
      ->getMock();

    $advdefMock->method('getFullPathToBinary')
      ->willReturn('/bin/advdef');
    $advdefMock->method('sanitizeFilename')
      ->will($this->returnArgument(0));

    $this->assertEquals('/bin/advdef', $advdefMock->getFullPathToBinary());
    $this->assertEquals('some filename', $advdefMock->sanitizeFilename('some filename'));

    $advdefMock->expects($this->once())
      ->method('execShellCommand')
      ->with($this->equalTo('something'));

    $advdefMock->applyToImage('public://test_image.png');

  }
}
