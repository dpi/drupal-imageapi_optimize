<?php

namespace Drupal\imageapi_optimize\Plugin\ImageAPIOptimizeProcessor;

use Drupal\Core\Form\FormStateInterface;
use Drupal\Core\Image\ImageInterface;
use Drupal\imageapi_optimize\ConfigurableImageAPIOptimizeProcessorBase;
use Drupal\imageapi_optimize\ImageAPIOptimizeProcessorBase;

/**
 * Desaturates (grayscale) an image resource.
 *
 * @ImageAPIOptimizeProcessor(
 *   id = "resmushit",
 *   label = @Translation("Resmush.it"),
 *   description = @Translation("Uses the free resmush.it service to optimize images.")
 * )
 */
class reSmushit extends ConfigurableImageAPIOptimizeProcessorBase {

  public function applyToImage($image_uri) {
    // @TODO: Catch exceptions, better error handling etc.


    // Need to send the file off to the remote service and await a response.
    $client = \Drupal::httpClient();
    $fields = [];
    $fields[] = [
      'name' => 'files',
      'contents' => fopen($image_uri, 'r'),
    ];
    if (!empty($this->configuration['quality'])) {
      $fields[] = [
        'name' => 'qlty',
        'contents' => $this->configuration['quality'],
      ];
    }

    $response = $client->post('http://api.resmush.it/ws.php', ['multipart' => $fields]);
    $body = $response->getBody();
    $json = json_decode($body);
    // If this has worked, we should get a dest entry in the JSON returned.
    if (isset($json->dest)) {
      // Now go fetch that, and save it locally.
      $smushedFile = $client->get($json->dest);
      if ($smushedFile->getStatusCode() == 200) {
        file_unmanaged_save_data($smushedFile->getBody(), $image_uri, FILE_EXISTS_REPLACE);
      }
      return TRUE;
    }

    return FALSE;
  }

  /**
   * {@inheritdoc}
   */
  public function defaultConfiguration() {
    return array(
      'quality' => NULL,
    );
  }

  /**
   * {@inheritdoc}
   */
  public function buildConfigurationForm(array $form, FormStateInterface $form_state) {
    $form['quality'] = array(
      '#type' => 'number',
      '#title' => t('JPG image quality'),
      '#default_value' => $this->configuration['quality'],
      '#min' => 1,
      '#max' => 100,
    );
    return $form;
  }

  /**
   * {@inheritdoc}
   */
  public function submitConfigurationForm(array &$form, FormStateInterface $form_state) {
    parent::submitConfigurationForm($form, $form_state);

    $this->configuration['quality'] = $form_state->getValue('quality');
  }

}
