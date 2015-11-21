<?php

/**
 */
interface ImageAPIOptimizeProcessorInterface {

  public function __construct($data);

  public function getDescription();

  public function process($image, $dst);
}