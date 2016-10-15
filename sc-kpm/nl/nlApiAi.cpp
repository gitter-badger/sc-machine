/*
* This source file is part of an OSTIS project. For the latest info, see http://ostis.net
* Distributed under the MIT License
* (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
*/

#include "nlApiAi.hpp"
#include "nlApiAiUtils.hpp"

#include "wrap/sc_stream.hpp"


namespace nl
{
	ScAddr AApiAiParseUserTextAgent::ms_nrelCommonTemplate;
	ScAddr AApiAiParseUserTextAgent::ms_nrelTranslation;
	ScAddr AApiAiParseUserTextAgent::ms_rrelLocation;

	/** 
	  * Command template: requestAddr _-> _text;;
	  */
	SC_AGENT_ACTION_IMPLEMENTATION(AApiAiParseUserTextAgent)
	{
		curl_global_init(CURL_GLOBAL_ALL);

		// get text content of argument
		ScIterator3Ptr iter = mMemoryCtx.iterator3(requestAddr, SC_TYPE(sc_type_arc_pos_const_perm), SC_TYPE(sc_type_link));
		if (iter->next())
		{
			ScStream stream;
			if (mMemoryCtx.getLinkContent(iter->value(2), stream))
			{
				std::string text;
				if (StreamConverter::streamToString(stream, text))
				{
					ApiAiRequestParams params;
					params.m_queryString = text;
					params.m_sessionId = "test_session";	/// TODO: Build session id based on user node
					params.m_lang = "en";	/// TODO: get from kb

					ApiAiRequest request;
				
					if (request.perform(params))
					{
						// try to find template for specified action
						const ApiAiRequestResult & result = request.getResult();
												
						std::string actionName = utils::StringUtils::replaceAll(result.GetAction(), ".", "_");						
						
						ScAddr actionAddr;
						if (mMemoryCtx.helperFindBySystemIdtf(std::string("apiai_action_") + actionName, actionAddr))
						{
							ScIterator5Ptr iterTempl = mMemoryCtx.iterator5(actionAddr, 
										*ScType::EDGE_DCOMMON_CONST,
										*ScType::NODE_CONST_STRUCT,
										*ScType::EDGE_ACCESS_CONST_POS_PERM,
										ms_nrelCommonTemplate
									);
							if (iterTempl->next())
							{
								ScTemplate templ;
								if (mMemoryCtx.helperBuildTemplate(templ, iterTempl->value(2)))
								{
									// setup template parameters
									ScTemplateGenParams params;
									//params.add()
								}
							}
						}
					}
				}
				else
				{
					return SC_RESULT_ERROR_IO;
				}
			}
			else
			{
				return SC_RESULT_ERROR_INVALID_STATE;
			}
		}

		return SC_RESULT_ERROR_INVALID_PARAMS;
	}
}