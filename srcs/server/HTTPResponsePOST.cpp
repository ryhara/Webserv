#include "HTTPResponse.hpp"


//  TODO : POSTはどの形式に対応するか、他者の挙動を確認する
// TODO : POSTはとりあえずファイルのアップロードとCGIに対応すればよいかもしれない
void HTTPResponse::makePostResponseBody(HTTPRequest &request)
{
	std::string path = request.getUri();

}
