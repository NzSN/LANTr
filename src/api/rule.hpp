#ifndef LANTR_API_RULE_H_
#define LANTR_API_RULE_H_

#include "base/types.hpp"
#include "base/utilities/assert.hpp"

namespace LANTr {
namespace Api {

namespace {
using Source = Base::Types::Source;
}

class RewriteRule {
public:

  static constexpr int num_of_minimum_components = 2;
  static constexpr int num_of_maximum_components = 3;

  RewriteRule(Source sPattern, Source tPattern, Source where):
    sPattern_(sPattern), tPattern_(tPattern), where_(where) {}

  RewriteRule(std::initializer_list<std::string> args) {
    ASSERT(args.size() > num_of_minimum_components ||
           args.size() < num_of_maximum_components,
           "Init a rule with " + std::to_string(args.size()) + " components is not allowed."
           + " Rewrite rule consist of at most of three components "
           "or at least two components.");

    auto iter = args.begin();
    sPattern_ = *(iter);
    tPattern_ = *(iter+1);

    if (args.size() == num_of_maximum_components) {
      where_ = *(iter+2);
    }
  }

  RewriteRule(const RewriteRule& other) noexcept {
    this->sPattern_ = other.sPattern_;
    this->tPattern_ = other.tPattern_;
    this->where_ = other.where_;
  }

  const Source source() const {
    return sPattern_;
  }

  const Source target() const {
    return tPattern_;
  }

  const Source where() const {
    return where_;
  }

private:
  Source sPattern_;
  Source tPattern_;
  Source where_;
};

} // Api
} // LANTr


#endif // LANTR_API_RULE_H_
