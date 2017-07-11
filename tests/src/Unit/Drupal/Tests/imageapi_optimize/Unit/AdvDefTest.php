<?php

namespace Drupal\Tests\imageapi_optimize\Unit;

use Drupal\Tests\UnitTestCase;
use Drupal\imageapi_optimize\Plugin\ImageAPIOptimizeProcessor\AdvDef;

/**
 * Tests AdvDef image optimize plugin.
 *
 * @group imageapi_optimize
 */
class AdvDefTest extends UnitTestCase {
  function testCase() {
    $this->assertTrue(TRUE);



    // Make some mocks.
    $config = [];
    $loggerMock = $this->getMock('\Psr\Log\LoggerInterface');
    $imageFactoryMock = $this->getMockBuilder('\Drupal\Core\Image\ImageFactory')
      ->disableOriginalConstructor()
      ->getMock();
    $fileSystemMock = $this->getMock('\Drupal\Core\File\FileSystemInterface');
    $shellOperationsMock = $this->getMockBuilder('\Drupal\iamgeapi_optimize\ImageAPIOptimizeShellOperationsInterface')
      ->setMethods(['findExecutablePath', 'execShellCommand'])
      ->getMock();

    // Load up the plugin.
    $advdef = new AdvDef($config, 'advdef', [], $loggerMock, $imageFactoryMock, $fileSystemMock, $shellOperationsMock);

    $this->assertArrayHasKey('recompress', $advdef->defaultConfiguration());
    $this->assertArrayHasKey('mode', $advdef->defaultConfiguration());

    $shellOperationsMock->method('findExecutablePath')->will($this->returnArgument(0));
    $fileSystemMock->method('realpath')->will($this->returnArgument(0));

    $imagePNGMock = $this->getMock('\Drupal\Core\Image\ImageInterface');
    $imagePNGMock->method('getMimeType')->willReturn('image/png');
    $imageFactoryMock->method('get')->willReturn($imagePNGMock);

    $shellOperationsMock->expects($this->once())
      ->method('execShellCommand')
      ->with(
        $this->equalTo('advdef'),
        $this->identicalTo(['--quiet', '--recompress', '-3']),
        $this->identicalTo(['public://test_image.png'])
      );

    $advdef->applyToImage('public://test_image.png');

  }
}
