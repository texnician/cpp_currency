#include <folly/futures/Future.h>
#include <iostream>
#include <vector>
#include <memory>
#include <string>

using namespace folly;
using namespace std;

void foo(int x) {
  // do something with x
  cout << "foo(" << x << ")" << endl;
}

enum class Result {
  SUCCESS,
  FAIL,
  TIMEOUT
};

struct CookieMapping {
  Result result;
  // The value when result is FOUND,
  // The error message when result is SERVER_ERROR or CLIENT_ERROR
  // undefined otherwise
  std::string mvid;
};

struct CookieLabel {
  Result result;
  // The value when result is FOUND,
  // The error message when result is SERVER_ERROR or CLIENT_ERROR
  // undefined otherwise
  std::vector<std::string> labels;
};

class AerospikeClient {
 public:
  folly::Future<CookieMapping> getCookieMapping(std::string tid)
    {
      std::cout << "Get cookie mapping, tid: " << tid << std::endl;
      CookieMapping cm;
      cm.result = Result::SUCCESS;
      cm.mvid = "abcdefgh";
      return folly::makeFuture(std::move(cm));
    }

  folly::Future<CookieLabel> getCookieLabel(std::string mvid)
    {
      std::cout << "Get cookie label, mvid: " << mvid << std::endl;
      CookieLabel cl;
      cl.result = Result::SUCCESS;
      cl.labels = { "123", "456", "789" };
      return folly::makeFuture(std::move(cl));
    }
};

struct CandidateResult {
  Result result;
  // The value when result is FOUND,
  // The error message when result is SERVER_ERROR or CLIENT_ERROR
  // undefined otherwise
  std::vector<std::string> candidates;
};

struct ISearchInfo {
  std::vector<std::string> labels;
};

class ISearchRootService {
 public:
  folly::Future<CandidateResult> getKwCandidate(ISearchInfo isearch_info)
    {
      std::cout << "Get keyword candidates: ";
      for (const auto& lb : isearch_info.labels) {
        std::cout << lb << ", ";
      }
      std::cout << std::endl;
      CandidateResult cand;
      cand.result = Result::SUCCESS;
      cand.candidates = { "hello", "world" };
      return folly::makeFuture(std::move(cand));
    }
};


class ISearchService {
 public:

  folly::Future<CandidateResult> getCandidate(ISearchInfo isearch_info)
    {
      std::cout << "Get normal candidates: ";
      for (const auto& lb : isearch_info.labels) {
        std::cout << lb << ", ";
      }
      std::cout << std::endl;
      CandidateResult cand;
      cand.result = Result::SUCCESS;
      cand.candidates = { "image", "flash" };
      return folly::makeFuture(std::move(cand));
    }
};

int main(int argc, char* argv[])
{
  cout << "making Promise" << endl;
  Promise<int> p;
  Future<int> f = p.getFuture();
  f.then(foo);
  cout << "Future chain made" << endl;

  // ... now perhaps in another event callback

  cout << "fulfilling Promise" << endl;
  p.setValue(42);
  cout << "Promise fulfilled" << endl;

  auto as = std::make_shared<AerospikeClient>();
  auto isv_root = std::make_shared<ISearchRootService>();
  auto isv = std::make_shared<ISearchService>();

  std::string tid = "12345";

  Future<CookieMapping> fut1 = as->getCookieMapping(tid);

  folly::Future<CookieLabel> fut2 = fut1.then([as](CookieMapping cm) {
      std::string mvid = cm.result == Result::SUCCESS ? cm.mvid : "";
      return as->getCookieLabel(mvid);
    }); 

  auto fut3 = fut2.then([isv_root, isv](CookieLabel cl) {
      // get cookie labels, make isearch info
      ISearchInfo info;
      info.labels = cl.labels;
      return folly::collectAll(isv_root->getKwCandidate(info), 
                               isv->getCandidate(info));
    });

  using CandiateTuple = decltype(fut3)::value_type;

  fut3.then([](CandiateTuple cands) {
      std::vector<std::string> kws = std::get<0>(cands).value().candidates;
      std::vector<std::string> normals = std::get<1>(cands).value().candidates;
      std::cout << "keyword candidates: " << kws.size() << std::endl;
      std::cout << "normal candidates: " << normals.size() << std::endl;
      // send response
    });
}
