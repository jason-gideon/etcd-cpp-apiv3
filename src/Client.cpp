#include <memory>
#include "etcd/Client.hpp"
#include "v3/include/action_constants.hpp"
#include "v3/include/AsyncTxnResponse.hpp"
#include "v3/include/AsyncRangeResponse.hpp"
#include "v3/include/AsyncWatchResponse.hpp"
#include "v3/include/Transaction.hpp"
#include <iostream>

#include "v3/include/AsyncSetAction.hpp"
#include "v3/include/AsyncCompareAndSwapAction.hpp"
#include "v3/include/AsyncCompareAndDeleteAction.hpp"
#include "v3/include/AsyncUpdateAction.hpp"
#include "v3/include/AsyncGetAction.hpp"
#include "v3/include/AsyncDeleteAction.hpp"
#include "v3/include/AsyncWatchAction.hpp"

using grpc::Channel;
using etcdserverpb::PutRequest;
using etcdserverpb::RangeRequest;
using etcdserverpb::TxnRequest;
using etcdserverpb::DeleteRangeRequest;
using etcdserverpb::Compare;
using etcdserverpb::RequestOp;


etcd::Client::Client(std::string const & address)
{
  std::string stripped_address(address);
  std::string substr("http://");
  std::string::size_type i = stripped_address.find(substr);
  if(i != std::string::npos)
  {
    stripped_address.erase(i,substr.length());
  }
  std::shared_ptr<Channel> channel = grpc::CreateChannel(stripped_address, grpc::InsecureChannelCredentials());
  stub_= KV::NewStub(channel);
  watchServiceStub= Watch::NewStub(channel);
}


pplx::task<etcd::Response> etcd::Client::get(std::string const & key)
{
  std::shared_ptr<etcdv3::AsyncGetAction> call(new etcdv3::AsyncGetAction(key,stub_.get()));
  return Response::create(call);
}

pplx::task<etcd::Response> etcd::Client::set(std::string const & key, std::string const & value)
{
  std::shared_ptr<etcdv3::AsyncSetAction> call(new etcdv3::AsyncSetAction(key, value, stub_.get()));
  return Response::create(call);;
}

pplx::task<etcd::Response> etcd::Client::add(std::string const & key, std::string const & value)
{
  std::shared_ptr<etcdv3::AsyncSetAction> call(new etcdv3::AsyncSetAction(key, value, stub_.get(), true));
  return Response::create(call);;
}

pplx::task<etcd::Response> etcd::Client::modify(std::string const & key, std::string const & value)
{
  std::shared_ptr<etcdv3::AsyncUpdateAction> call(new etcdv3::AsyncUpdateAction(key,value,stub_.get()));;
  return Response::create(call);
}


pplx::task<etcd::Response> etcd::Client::modify_if(std::string const & key, std::string const & value, std::string const & old_value)
{
  std::shared_ptr<etcdv3::AsyncCompareAndSwapAction> call(new etcdv3::AsyncCompareAndSwapAction(key,value,old_value, stub_.get()));;
  return Response::create(call);
}


pplx::task<etcd::Response> etcd::Client::modify_if(std::string const & key, std::string const & value, int old_index)
{
  std::shared_ptr<etcdv3::AsyncCompareAndSwapAction> call(new etcdv3::AsyncCompareAndSwapAction(key,value,old_index, stub_.get()));;
  return Response::create(call);
}


pplx::task<etcd::Response> etcd::Client::rm(std::string const & key)
{
  std::shared_ptr<etcdv3::AsyncDeleteAction> call(new etcdv3::AsyncDeleteAction(key,stub_.get()));;
  return Response::create(call);
}


pplx::task<etcd::Response> etcd::Client::rm_if(std::string const & key, std::string const & old_value)
{
  std::shared_ptr<etcdv3::AsyncCompareAndDeleteAction> call(new etcdv3::AsyncCompareAndDeleteAction(key,old_value,stub_.get()));;
  return Response::create(call);
}

pplx::task<etcd::Response> etcd::Client::rm_if(std::string const & key, int old_index)
{
  std::shared_ptr<etcdv3::AsyncCompareAndDeleteAction> call(new etcdv3::AsyncCompareAndDeleteAction(key,old_index,stub_.get()));;
  return Response::create(call);

}

pplx::task<etcd::Response> etcd::Client::rmdir(std::string const & key, bool recursive)
{
  std::shared_ptr<etcdv3::AsyncDeleteAction> call(new etcdv3::AsyncDeleteAction(key,stub_.get(),true));
  return Response::create(call);
}

pplx::task<etcd::Response> etcd::Client::ls(std::string const & key)
{

  std::shared_ptr<etcdv3::AsyncGetAction> call(new etcdv3::AsyncGetAction(key,stub_.get(),true));
  return Response::create(call);
}

pplx::task<etcd::Response> etcd::Client::watch(std::string const & key, bool recursive)
{
  std::shared_ptr<etcdv3::AsyncWatchAction> call(new etcdv3::AsyncWatchAction(key,recursive,stub_.get(),watchServiceStub.get()));
  return Response::create(call);
}

pplx::task<etcd::Response> etcd::Client::watch(std::string const & key, int fromIndex, bool recursive)
{
  std::shared_ptr<etcdv3::AsyncWatchAction> call(new etcdv3::AsyncWatchAction(key,fromIndex,recursive,stub_.get(),watchServiceStub.get()));
  return Response::create(call);
}


