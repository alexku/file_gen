<?php

/**
 * @file
 *   A drush command generating a file from a template.
 *
 */

/**
 * Implementation of hook_drush_command().
 *
 * @return
 *   An associative array describing our command(s).
 */
function file_gen_drush_command() {
  $items = array();

  // The 'file-generate' command
  $items['file-generate'] = array(
    'description' => "Generate a file based on template. All command line arguments will become tokens in the template (e.g. ![site-name]).",
    'arguments' => array(
      'template-path' => 'Path to the template file (defaults to default.tpl.txt in the directory of file_gen)',
    ),
    'options' => array(
      'deploy-repository=git@some.host:some/repo.git' => 'Value to use for repository.',
    ),
    'examples' => array(
      'drush file-generate default.tpl.txt' => 'Generate a file from default.tpl.txt template.',
    ),
    'aliases' => array(),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH, // No bootstrap at all.
  );

  return $items;
}

/**
 * Implementation of hook_drush_help().
 *
 * @param
 *   A string with the help section (prepend with 'drush:')
 *
 * @return
 *   A string with the help text for your command.
 */
function file_gen_drush_help($section) {
  switch ($section) {
    case 'drush:file-generate':
      return dt("A command to generate a file based on template. All command line arguments will become tokens in the template (e.g. ![site-name]).");
  }
}

/**
 * Drush command callback. This is where the action takes place.
 * 
 * @param $site_name
 *   A string with value for sitename.
 */
function drush_file_gen_file_generate($template_path) {
  if (!isset($template_path)) {
    drush_set_error('DRUSH_VHOST_GEN_ARG_FAIL',
      dt('You have to provide an argument for template path.'));
  }
  else {
    $options = array(
      'template-path' => $template_path,
    );

    // get all command line args
    $cli_options = drush_get_context('cli');
    foreach ($cli_options as $option => $value) {
      $options[$option] = $value;
    }
    drush_file_gen_generate($options);
  }
}

/**
 * Generate a file using a template.
 * 
 * @param $options
 *   An array with values for different options.
 */
function drush_file_gen_generate($options) {
  $template_path = $options['template-path'];

  // if full path was not provided, look in the directory of alias_gen
  if (strpos($template_path, '/') !== 0) {
    $template_path = dirname(__FILE__) . '/' . $template_path;
  }

  $template = file_get_contents($template_path);

  $needles = array();
  $replacements = array();
  foreach ($options as $option_name => $option_value) {
    // $needles[] = "![{$option_name}]";
    $needles[] = "{{ {$option_name} }}";
    $replacements[] = $option_value;
  }
  $result = str_replace($needles, $replacements, $template);

  drush_print($result);
}
