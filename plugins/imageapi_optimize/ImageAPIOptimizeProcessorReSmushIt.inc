<?php

class ImageAPIOptimizeProcessorReSmushIt {

  protected $db_table = 'imageapi_optimize_pipeline_processor';

  function load_row($data) {
    $schema = drupal_get_schema($this->db_table);

    // Go through our schema and build correlations.
    foreach ($schema['fields'] as $field => $info) {
      $this->$field = empty($info['serialize']) ? $data->$field : unserialize($data->$field);
    }
  }
}