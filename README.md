# Lamping's Abstract Algorithm

A cleaned up adaptation of [Lamping's abstract algorithm](http://dl.acm.org/citation.cfm?id=96711). It evaluates functions optimally by encoding a λ-term as ([symmetric](https://scholar.google.com.br/scholar?q=symmetric+interaction+combinators&hl=en&as_sdt=0&as_vis=1&oi=scholart&sa=X&ved=0ahUKEwjNgZbO7aTVAhUEkZAKHYyTAFgQgQMIJjAA)) [interaction combinators](http://www.sciencedirect.com/science/article/pii/S0890540197926432), normalizing the resulting graph, and decoding it back. It asymptotically beats all usual evaluators of functional programs, including Haskell's GHC, Scheme compilers, Google's V8 and so on. Moreover, it is capable of automatically exploiting any inherent parallelizability of your program, since interaction nets are a naturally concurrent model of computation. This implementation consists of two files, [a 121-loc reducer](src/interaction-combinators.js) for interaction combinators and [a 71-loc conversor](https://github.com/MaiaVictor/abstract-algorithm/blob/master/src/lambda-encoder.js) of λ-terms to and from those. It isn't parallel, but is completely lazy: it doesn't perform any work that won't influence on the normal form. There is an inherent tradeoff between those.

![combinator_rules](images/combinators_rules.png)

## Usage

- Install

    ```bash
    npm install -g abstract-algorithm
    ```

- Use as a lib

    ```javascript
    const absal = require(".");

    // church-encoded exponentiation of 2^2
    const term = `
      @two #s #z /s /s z
      /two two
    `;

    // evaluates optimally
    console.log(absal.reduce(term,1)); // 1 = return stats
    ```

- Use as a command

    ```bash
    absal fileName.lam
    ```

## Bla bla bla

Crappy handwritten examples:

- [Numbers](images/handwritten_example.jpg?raw=true)

- [Pairs](images/pairs_on_inets.jpg?raw=true)

- [Either](images/either_on_inets.jpg?raw=true)

[Here](https://github.com/MaiaVictor/abstract-algorithm/blob/old_repo/examples/lambda-calculus.js) is a working example on how to use this lib to reduce untyped λ-terms (outdated syntax). [Here](https://github.com/MaiaVictor/parallel_lambda_computer_tests) is an attempt to flatten the graph-reduction algorithm to a linear automata, in such a way that it could, in theory, be implemented as a massivelly parallel ASIC where each 128-bit memory cell is actually a nano-CPU capable of updating its state by looking at its neighbors, in such a way that it causes the emergent global behavior of the memory to converge to the normal form of the initial graph.

~~Note this only works with [stratified terms](https://www.reddit.com/r/haskell/comments/6phxvb/cleaned_up_implementation_of_lampings_abstract/dkq57yx/?context=1), which roughly means that the algorithm breaks when you have duplications inside of duplications. To make it work with arbitrary terms, you'd need to augment the system with a complex machinery known as oracle. There are many ways to do it, but all of them ruin the performance. I personally prefer to call non-stratified terms defective and ignore them. It is not hard to avoid those terms, you just have to be cautious about the implicit duplication of arguments performed by beta-reduction (which is the root of all evil).~~ (Edit: [this is outdated](https://www.reddit.com/r/haskell/comments/8bwlxp/supercompilation_for_free_with_the_abstract/dxaiuyi/); the abstract algorithm seems to work fine with, for example, general recursion, so stratification is too restrict. It is unclear what is a precise structural criteria that determines terms that work, but, in my experience, any Haskell declaration works with some minor tweaks. Note that the subset of oracle-free terms [is Turing complete](https://github.com/MaiaVictor/articles/tree/master/0000-oracle-free-terms-are-turing-complete).)
