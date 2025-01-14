/**
 * Autogenerated by Thrift
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */

#include <src/gen-py3/module/clients_wrapper.h>

namespace cpp2 {


folly::Future<bool>
MyServiceClientWrapper::hasDataById(
    apache::thrift::RpcOptions& rpcOptions,
    int64_t arg_id) {
  auto* client = static_cast<::cpp2::MyServiceAsyncClient*>(async_client_.get());
  folly::Promise<bool> _promise;
  auto _future = _promise.getFuture();
  auto callback = std::make_unique<::thrift::py3::FutureCallback<bool>>(
    std::move(_promise), rpcOptions, client->recv_wrapped_hasDataById, channel_);
  try {
    client->hasDataById(
      rpcOptions,
      std::move(callback),
      arg_id
    );
  } catch (const std::exception& ex) {
    return folly::makeFuture<bool>(folly::exception_wrapper(
      std::current_exception(), ex
    ));
  }
  return _future;
}

folly::Future<std::string>
MyServiceClientWrapper::getDataById(
    apache::thrift::RpcOptions& rpcOptions,
    int64_t arg_id) {
  auto* client = static_cast<::cpp2::MyServiceAsyncClient*>(async_client_.get());
  folly::Promise<std::string> _promise;
  auto _future = _promise.getFuture();
  auto callback = std::make_unique<::thrift::py3::FutureCallback<std::string>>(
    std::move(_promise), rpcOptions, client->recv_wrapped_getDataById, channel_);
  try {
    client->getDataById(
      rpcOptions,
      std::move(callback),
      arg_id
    );
  } catch (const std::exception& ex) {
    return folly::makeFuture<std::string>(folly::exception_wrapper(
      std::current_exception(), ex
    ));
  }
  return _future;
}

folly::Future<folly::Unit>
MyServiceClientWrapper::putDataById(
    apache::thrift::RpcOptions& rpcOptions,
    int64_t arg_id,
    std::string arg_data) {
  auto* client = static_cast<::cpp2::MyServiceAsyncClient*>(async_client_.get());
  folly::Promise<folly::Unit> _promise;
  auto _future = _promise.getFuture();
  auto callback = std::make_unique<::thrift::py3::FutureCallback<folly::Unit>>(
    std::move(_promise), rpcOptions, client->recv_wrapped_putDataById, channel_);
  try {
    client->putDataById(
      rpcOptions,
      std::move(callback),
      arg_id,
      arg_data
    );
  } catch (const std::exception& ex) {
    return folly::makeFuture<folly::Unit>(folly::exception_wrapper(
      std::current_exception(), ex
    ));
  }
  return _future;
}

folly::Future<folly::Unit>
MyServiceClientWrapper::lobDataById(
    apache::thrift::RpcOptions& rpcOptions,
    int64_t arg_id,
    std::string arg_data) {
  auto* client = static_cast<::cpp2::MyServiceAsyncClient*>(async_client_.get());
  folly::Promise<folly::Unit> _promise;
  auto _future = _promise.getFuture();
  auto callback = std::make_unique<::apache::thrift::OneWayFutureCallback>(
    std::move(_promise), channel_);
  try {
    client->lobDataById(
      rpcOptions,
      std::move(callback),
      arg_id,
      arg_data
    );
  } catch (const std::exception& ex) {
    return folly::makeFuture<folly::Unit>(folly::exception_wrapper(
      std::current_exception(), ex
    ));
  }
  return _future;
}

folly::Future<bool>
MyServiceFastClientWrapper::hasDataById(
    apache::thrift::RpcOptions& rpcOptions,
    int64_t arg_id) {
  auto* client = static_cast<::cpp2::MyServiceFastAsyncClient*>(async_client_.get());
  folly::Promise<bool> _promise;
  auto _future = _promise.getFuture();
  auto callback = std::make_unique<::thrift::py3::FutureCallback<bool>>(
    std::move(_promise), rpcOptions, client->recv_wrapped_hasDataById, channel_);
  try {
    client->hasDataById(
      rpcOptions,
      std::move(callback),
      arg_id
    );
  } catch (const std::exception& ex) {
    return folly::makeFuture<bool>(folly::exception_wrapper(
      std::current_exception(), ex
    ));
  }
  return _future;
}

folly::Future<std::string>
MyServiceFastClientWrapper::getDataById(
    apache::thrift::RpcOptions& rpcOptions,
    int64_t arg_id) {
  auto* client = static_cast<::cpp2::MyServiceFastAsyncClient*>(async_client_.get());
  folly::Promise<std::string> _promise;
  auto _future = _promise.getFuture();
  auto callback = std::make_unique<::thrift::py3::FutureCallback<std::string>>(
    std::move(_promise), rpcOptions, client->recv_wrapped_getDataById, channel_);
  try {
    client->getDataById(
      rpcOptions,
      std::move(callback),
      arg_id
    );
  } catch (const std::exception& ex) {
    return folly::makeFuture<std::string>(folly::exception_wrapper(
      std::current_exception(), ex
    ));
  }
  return _future;
}

folly::Future<folly::Unit>
MyServiceFastClientWrapper::putDataById(
    apache::thrift::RpcOptions& rpcOptions,
    int64_t arg_id,
    std::string arg_data) {
  auto* client = static_cast<::cpp2::MyServiceFastAsyncClient*>(async_client_.get());
  folly::Promise<folly::Unit> _promise;
  auto _future = _promise.getFuture();
  auto callback = std::make_unique<::thrift::py3::FutureCallback<folly::Unit>>(
    std::move(_promise), rpcOptions, client->recv_wrapped_putDataById, channel_);
  try {
    client->putDataById(
      rpcOptions,
      std::move(callback),
      arg_id,
      arg_data
    );
  } catch (const std::exception& ex) {
    return folly::makeFuture<folly::Unit>(folly::exception_wrapper(
      std::current_exception(), ex
    ));
  }
  return _future;
}

folly::Future<folly::Unit>
MyServiceFastClientWrapper::lobDataById(
    apache::thrift::RpcOptions& rpcOptions,
    int64_t arg_id,
    std::string arg_data) {
  auto* client = static_cast<::cpp2::MyServiceFastAsyncClient*>(async_client_.get());
  folly::Promise<folly::Unit> _promise;
  auto _future = _promise.getFuture();
  auto callback = std::make_unique<::apache::thrift::OneWayFutureCallback>(
    std::move(_promise), channel_);
  try {
    client->lobDataById(
      rpcOptions,
      std::move(callback),
      arg_id,
      arg_data
    );
  } catch (const std::exception& ex) {
    return folly::makeFuture<folly::Unit>(folly::exception_wrapper(
      std::current_exception(), ex
    ));
  }
  return _future;
}

folly::Future<std::string>
DbMixedStackArgumentsClientWrapper::getDataByKey0(
    apache::thrift::RpcOptions& rpcOptions,
    std::string arg_key) {
  auto* client = static_cast<::cpp2::DbMixedStackArgumentsAsyncClient*>(async_client_.get());
  folly::Promise<std::string> _promise;
  auto _future = _promise.getFuture();
  auto callback = std::make_unique<::thrift::py3::FutureCallback<std::string>>(
    std::move(_promise), rpcOptions, client->recv_wrapped_getDataByKey0, channel_);
  try {
    client->getDataByKey0(
      rpcOptions,
      std::move(callback),
      arg_key
    );
  } catch (const std::exception& ex) {
    return folly::makeFuture<std::string>(folly::exception_wrapper(
      std::current_exception(), ex
    ));
  }
  return _future;
}

folly::Future<std::string>
DbMixedStackArgumentsClientWrapper::getDataByKey1(
    apache::thrift::RpcOptions& rpcOptions,
    std::string arg_key) {
  auto* client = static_cast<::cpp2::DbMixedStackArgumentsAsyncClient*>(async_client_.get());
  folly::Promise<std::string> _promise;
  auto _future = _promise.getFuture();
  auto callback = std::make_unique<::thrift::py3::FutureCallback<std::string>>(
    std::move(_promise), rpcOptions, client->recv_wrapped_getDataByKey1, channel_);
  try {
    client->getDataByKey1(
      rpcOptions,
      std::move(callback),
      arg_key
    );
  } catch (const std::exception& ex) {
    return folly::makeFuture<std::string>(folly::exception_wrapper(
      std::current_exception(), ex
    ));
  }
  return _future;
}

} // namespace cpp2
