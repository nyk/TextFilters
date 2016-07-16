<?php
/**
 * Helper class to assert clean text rules;
 *
 * Filter methods in the class are all designed to be chained so that
 * multiple filter operations can be applied in any order.
 *
 * Example usage:
 *   use TextFilters as TF; // alias the namespace
 *   $ugly = "  this is  my ugly  text   ";
 *   $pretty = TF\Filter::input($ugly)->whitespace()->sentence_case()->fullstop()->output();
 *   echo $pretty; // prints "This is my ugly text."
 *
 * @author Nyk Cowham <nyk@demotix.com>
 * @author Nuno Veloso <nuno@demotix.com>
 *
 * @version 2.0
 */

namespace TextFilters;

class Filter
{
  public    $encoding;
  protected $text;

  public function __construct($text = "", $enc = 'UTF-8') {
    $this->text = $text;
    $this->encoding = $enc;
  }


  /**
   * A static factory method for creating new chainable TextFilters\Filter objects.
   *
   * @param
   *   String containing input text to be filtered.
   *
   * @returns
   *   An initialized TextFilters\Filter object.
   */
  public static function input($text) {
    return new Filter($text);
  }


  /**
   * Chainable filter method to strip redundant white space (leading, trailing and between words).
   *
   * @returns
   *   Initialized TextFilters\Filter object (chainable).
   */
  public function whitespace() {
    if ($this->text) {
       // strip n > 1 spaces between words.
       $this->text = preg_replace('/\s\s+/', ' ', $this->text);
       // strip any remaining leading or trailing whitespace.
       $this->text = trim($this->text);
    }
    return $this; // makes the filter chainable
  }


  /**
   * Chainable filter method to make text sentence case. This is not simply the capitalization of the
   * first word, but also capitalizes the first word following a fullstop and zero or more spaces.
   *
   * @returns
   *   Initialized TextFilters\Filter object (chainable).
   */
  public function sentenceCase() {

    if ($this->text) {
      // capitalise the first sentence
      if (function_exists('mb_strtoupper')) {
        $this->text = mb_strtoupper(mb_substr($this->text, 0, 1, $this->encoding)) . mb_substr($this->text, 1);
      } else {
        $this->text = ucfirst($this->text);
      }

      // capitalise the rest of the text
      $this->text = preg_replace('/([^\.])(\w\.\W*)([a-z])/me', "'\\1\\2'.strtoupper('\\3')", $this->text);

    }

    return $this;
  }

  public function titleCase() {
    if (function_exists('mb_convert_case')) {
      $this->text = mb_convert_case($this->text, MB_CASE_TITLE, $this->encoding);
    } else {
      $this->text = ucwords($this->text);
    }

    return $this;
  }

  /**
   * Chainable filter method to capitalize city names correctly.
   *
   * @param
   *   variadic list of arguments of (lower case) ISO language codes.
   *   E.g: ->location('en','fr','de') - these determine lower case rules
   *   applied to prepositions of place used in place names.
   *
   * @return
   *   Initialized TextFilters\Filter object (chainable).
   */
  public function location() {

    $langs = func_num_args() > 0
           ? func_get_args()
           : true;

    if (function_exists('mb_convert_case')) {
      $this->text = mb_convert_case($this->text, MB_CASE_TITLE, "UTF-8");
    }

    $preps = array('in');

    if ($langs OR in_array('en', $langs)) {
      array_push($preps, 'on','upon','under','over','of','by','the', 'and');
    }

    if ($langs OR in_array('es', $langs) OR in_array('it', $langs)) {
      array_push($preps, 'del','dell','dela','della');
    }

    if ($langs OR in_array('pt', $langs)) {
      array_push($preps, 'do','da','de');
    }

    if ($langs OR in_array('it', $langs)) {
      array_push($preps, 'della', 'di', 'sul', 'ed', 'a', 'e', 'dei','d\'');
    }

    if ($langs OR in_array('fr', $langs)) {
      array_push($preps, 'de','des','sur','le','la','dela');
    }

    if ($langs OR in_array('de', $langs)) {
      array_push($preps, 'an','am','im','bei','der');
    }

    $patterns = join('|', $preps);
    $this->text = ucfirst(preg_replace("/\b({$patterns})\b/mei", "strtolower('\\1')", $this->text));

    return $this;
  }

  /**
   * Chainable filter method to apply an array of regex replacement rules.
   *
   * @param
   *   array of Rule and FilterRule objects with the following possible attributes:
   *
   * @param
   *   boolean to indicate if this is a prefilter invocation or not. False is the default.
   *
   * @return
   *   Initialized TextFilters\Filter object (chainable).
   */ 
  public function rules($rules) {
    foreach ($rules as $rule) {

      $rule->preProcess($this->text);

      if ($rule->regex == 'filter') {
        call_user_func(array($this, $rule->replacement));
      }
      else {
        $this->text = preg_replace($rule->regex, $rule->replacement, $this->text);
      }

      $rule->postProcess($this->text);
    }

    return $this;
  }

  /**
   * Chainable filter method to end final sentence with a full stop / period.
   *
   * @returns
   *   Initialized TextFilters\Filter object (chainable).
   */
  public function punctuation() {

    if ($this->text) {

      // if it starts with an initial "." - remove
      if (substr($this->text, 0, 1) === '.') {
        $this->text = substr($this->text, 1);
      }

      // ".." and ",." => ".."
      $this->text = preg_replace('/[\.,]\./', '..', $this->text);

      // ".." and ".," => ".."
      $this->text = preg_replace('/\.[\.,]/', '..', $this->text);

      // Convert 3+ dots into 3 dots
      $this->text = preg_replace('/\.\.\.\.+/', '...', $this->text);

      // Convert 1-2 dots into 1 dot
      $this->text = preg_replace('/([^\.])\.\.([^\.])/', '\\1.\\2', $this->text);
      // special case for the end of line
      $this->text = preg_replace('/([^\.])\.\.$/', '\\1.', $this->text);

      // add a single space after any fullstop or coma and remove any space before it
      $this->text = preg_replace('/([^\dA-Z])\s*(\.\.)?([\.,])\s*([^\d])/', "\\1\\2\\3 \\4", $this->text);

      // re-trim it
      $this->text = trim($this->text);

      // end final sentence with a full stop / period.
      if (substr($this->text, -1) !== '.') {
        $this->text .= '.';
      }
    }

    return $this;
  }


  /**
   * Chainable filter method to strip out fullstops from the end of last sentence.
   *
   * @returns
   *   Initialized TextFilters\Filter object (chainable).
   */
   public function noFullStop() {
     if ($this->text) $this->text = preg_replace('/\.$/', '', $this->text);
     return $this;
   }


  /**
   * Chainable filter method to deal with brackets.
   *
   * @return Initialized TextFilters\Filter object (chainable).
   */
  public function brackets() {

    if ($this->text) {
      // spaces on opening brackets + capitalise
      $this->text = preg_replace('/\s*([\(\[\{])\s*/', " \\1", $this->text);
      // spaces on closing brackets
      $this->text = preg_replace('/\s*([\)\]\}])\s*/', '\\1 ', $this->text);

      // capitalise after a curve bracket
      $this->text = preg_replace('/\((.)/me', "strtoupper('(\\1')", $this->text);
    }

    return $this;
  }


  /**
   * Chainable filter method to deal with over capitalised text.
   *
   * @return Initialized TextFilters\Filter object (chainable).
   */
  public function lowPercentUp($percent = 70) {

    if ($this->text) {
      preg_match_all('/[A-Z]/', $this->text, $matches) ;
      $uc_in_string = count($matches[0]);
      $c_in_string = strlen($this->text);
      $ratio = round($uc_in_string / $c_in_string * 100);
      if ($ratio >= $percent) {
        $this->text = strtolower($this->text);
      }
    }

    return $this;
  }


  /**
   * Chainable filter method to deal with cleanup of sepacial cases.
   *
   * @return Initialized TextFilters\Filter object (chainable).
   */
  public function cleanup() {

    if ($this->text) {
      // If there was bad escaping, clean it up!
      $this->text = preg_replace('/\\\"/', '"', $this->text);
    }

    return $this;
  }


  /**
   * Accessor method to retrieve the final filtered text from the filter chain.
   *
   * @returns
   *   String representing the filtered text.
   */
  public function output() {
    return $this->text;
  }
}
