/*
 * Copyright (C) 2021 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __RFX_TOK_UTILS_H__
#define __RFX_TOK_UTILS_H__

class RfxTokUtils {
  private:
    RfxTokUtils();
    virtual ~RfxTokUtils();

  public:
    static int at_tok_start(char** p_cur);
    static int at_tok_char(char** p_cur);
    static int at_tok_nextint(char** p_cur, int* p_out);
    static int at_tok_nexthexint(char** p_cur, int* p_out);

    static int at_tok_nextbool(char** p_cur, char* p_out);
    static int at_tok_nextstr(char** p_cur, char** out);

    static int at_tok_hasmore(char** p_cur);

    static int at_tok_equal(char** p_cur);
    static int at_tok_nextlonglong(char** p_cur, long long* p_out);

  private:
    static void skipWhiteSpace(char** p_cur);
    static void skipNextComma(char** p_cur);
    static char* nextTok(char** p_cur);
    static int at_tok_nextint_base(char** p_cur, int* p_out, int base, int uns);
    static int at_tok_nextlonglong_base(char** p_cur, long long* p_out, int base, int uns);
};
#endif
