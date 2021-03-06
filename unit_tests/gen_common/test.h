/*
 * Copyright (C) 2017-2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#pragma once
#include "runtime/gen_common/hw_cmds.h"
#include "runtime/helpers/hw_info.h"

#include "gtest/gtest.h"
#include "igfxfmid.h"
#include "test_mode.h"

#include <cstdint>
#include <memory>
#include <unordered_set>

extern PRODUCT_FAMILY productFamily;
extern GFXCORE_FAMILY renderCoreFamily;

#ifdef TESTS_GEN8
#define BDW_TYPED_TEST_BODY testBodyHw<typename NEO::GfxFamilyMapper<IGFX_GEN8_CORE>::GfxFamily>();
#define BDW_TYPED_CMDTEST_BODY runCmdTestHwIfSupported<typename NEO::GfxFamilyMapper<IGFX_GEN8_CORE>::GfxFamily>();
#else
#define BDW_TYPED_TEST_BODY
#define BDW_TYPED_CMDTEST_BODY
#endif
#ifdef TESTS_GEN9
#define SKL_TYPED_TEST_BODY testBodyHw<typename NEO::GfxFamilyMapper<IGFX_GEN9_CORE>::GfxFamily>();
#define SKL_TYPED_CMDTEST_BODY runCmdTestHwIfSupported<typename NEO::GfxFamilyMapper<IGFX_GEN9_CORE>::GfxFamily>();
#else
#define SKL_TYPED_TEST_BODY
#define SKL_TYPED_CMDTEST_BODY
#endif
#ifdef TESTS_GEN10
#define CNL_TYPED_TEST_BODY testBodyHw<typename NEO::GfxFamilyMapper<IGFX_GEN10_CORE>::GfxFamily>();
#define CNL_TYPED_CMDTEST_BODY runCmdTestHwIfSupported<typename NEO::GfxFamilyMapper<IGFX_GEN10_CORE>::GfxFamily>();
#else
#define CNL_TYPED_TEST_BODY
#define CNL_TYPED_CMDTEST_BODY
#endif
#ifdef TESTS_GEN11
#define ICL_TYPED_TEST_BODY testBodyHw<typename NEO::GfxFamilyMapper<IGFX_GEN11_CORE>::GfxFamily>();
#define ICL_TYPED_CMDTEST_BODY runCmdTestHwIfSupported<typename NEO::GfxFamilyMapper<IGFX_GEN11_CORE>::GfxFamily>();
#else
#define ICL_TYPED_TEST_BODY
#define ICL_TYPED_CMDTEST_BODY
#endif

#define TO_STR2(x) #x
#define TO_STR(x) TO_STR2(x)

#define TEST_MAX_LENGTH 140
#define CHECK_TEST_NAME_LENGTH_WITH_MAX(test_fixture, test_name, max_length) \
    static_assert((NEO::defaultTestMode != NEO::TestMode::AubTests && NEO::defaultTestMode != NEO::TestMode::AubTestsWithTbx) || (sizeof(#test_fixture) + sizeof(#test_name) <= max_length), "Test and fixture names length exceeds max allowed size: " TO_STR(max_length));
#define CHECK_TEST_NAME_LENGTH(test_fixture, test_name) \
    CHECK_TEST_NAME_LENGTH_WITH_MAX(test_fixture, test_name, TEST_MAX_LENGTH)

#ifdef TEST_F
#undef TEST_F
#endif

// Taken from gtest.h
#define TEST_F(test_fixture, test_name)                \
    CHECK_TEST_NAME_LENGTH(test_fixture, test_name)    \
    GTEST_TEST_(test_fixture, test_name, test_fixture, \
                ::testing::internal::GetTypeId<test_fixture>())

#ifdef TEST_P
#undef TEST_P
#endif

// Taken from gtest.h
#define TEST_P(test_suite_name, test_name)                                                                                                                \
    CHECK_TEST_NAME_LENGTH(test_fixture, test_name)                                                                                                       \
    class GTEST_TEST_CLASS_NAME_(test_suite_name, test_name)                                                                                              \
        : public test_suite_name {                                                                                                                        \
      public:                                                                                                                                             \
        GTEST_TEST_CLASS_NAME_(test_suite_name, test_name)                                                                                                \
        () {}                                                                                                                                             \
        virtual void TestBody();                                                                                                                          \
                                                                                                                                                          \
      private:                                                                                                                                            \
        static int AddToRegistry() {                                                                                                                      \
            ::testing::UnitTest::GetInstance()->parameterized_test_registry().GetTestCasePatternHolder<test_suite_name>(                                  \
                                                                                 #test_suite_name, ::testing::internal::CodeLocation(__FILE__, __LINE__)) \
                ->AddTestPattern(                                                                                                                         \
                    #test_suite_name,                                                                                                                     \
                    #test_name,                                                                                                                           \
                    new ::testing::internal::TestMetaFactory<                                                                                             \
                        GTEST_TEST_CLASS_NAME_(test_suite_name, test_name)>());                                                                           \
            return 0;                                                                                                                                     \
        }                                                                                                                                                 \
        static int gtest_registering_dummy_;                                                                                                              \
        GTEST_DISALLOW_COPY_AND_ASSIGN_(                                                                                                                  \
            GTEST_TEST_CLASS_NAME_(test_suite_name, test_name));                                                                                          \
    };                                                                                                                                                    \
    int GTEST_TEST_CLASS_NAME_(test_suite_name,                                                                                                           \
                               test_name)::gtest_registering_dummy_ =                                                                                     \
        GTEST_TEST_CLASS_NAME_(test_suite_name, test_name)::AddToRegistry();                                                                              \
    void GTEST_TEST_CLASS_NAME_(test_suite_name, test_name)::TestBody()

// Macros to provide template based testing.
// Test can use FamilyType in the test -- equivalent to SKLFamily
#define HWTEST_TEST_(test_suite_name, test_name, parent_class, parent_id)                       \
    CHECK_TEST_NAME_LENGTH(test_suite_name, test_name)                                          \
    class PLATFORM_EXCLUDES_CLASS_NAME(test_suite_name, test_name) {                            \
      public:                                                                                   \
        static std::unique_ptr<std::unordered_set<uint32_t>> &getExcludes() {                   \
            static std::unique_ptr<std::unordered_set<uint32_t>> excludes;                      \
            return excludes;                                                                    \
        }                                                                                       \
        static void addExclude(uint32_t product) {                                              \
            auto &excludes = getExcludes();                                                     \
            if (excludes == nullptr) {                                                          \
                excludes = std::make_unique<std::unordered_set<uint32_t>>();                    \
            }                                                                                   \
            excludes->insert(product);                                                          \
        }                                                                                       \
    };                                                                                          \
                                                                                                \
    class GTEST_TEST_CLASS_NAME_(test_suite_name, test_name) : public parent_class {            \
                                                                                                \
      public:                                                                                   \
        GTEST_TEST_CLASS_NAME_(test_suite_name, test_name)                                      \
        () {}                                                                                   \
                                                                                                \
      private:                                                                                  \
        template <typename FamilyType>                                                          \
        void testBodyHw();                                                                      \
        bool notExcluded() const {                                                              \
            using ExcludesT = PLATFORM_EXCLUDES_CLASS_NAME(test_suite_name, test_name);         \
            auto &excludes = ExcludesT::getExcludes();                                          \
            if (excludes == nullptr) {                                                          \
                return true;                                                                    \
            }                                                                                   \
            return excludes->count(::productFamily) == 0;                                       \
        }                                                                                       \
        void SetUp() override {                                                                 \
            if (notExcluded()) {                                                                \
                parent_class::SetUp();                                                          \
            }                                                                                   \
        }                                                                                       \
        void TearDown() override {                                                              \
            if (notExcluded()) {                                                                \
                parent_class::TearDown();                                                       \
            }                                                                                   \
        }                                                                                       \
                                                                                                \
        void TestBody() override {                                                              \
            if (notExcluded()) {                                                                \
                switch (::renderCoreFamily) {                                                   \
                case IGFX_GEN8_CORE:                                                            \
                    BDW_TYPED_TEST_BODY                                                         \
                    break;                                                                      \
                case IGFX_GEN9_CORE:                                                            \
                    SKL_TYPED_TEST_BODY                                                         \
                    break;                                                                      \
                case IGFX_GEN10_CORE:                                                           \
                    CNL_TYPED_TEST_BODY                                                         \
                    break;                                                                      \
                case IGFX_GEN11_CORE:                                                           \
                    ICL_TYPED_TEST_BODY                                                         \
                    break;                                                                      \
                default:                                                                        \
                    ASSERT_TRUE((false && "Unknown hardware family"));                          \
                    break;                                                                      \
                }                                                                               \
            }                                                                                   \
        }                                                                                       \
        static ::testing::TestInfo *const test_info_ GTEST_ATTRIBUTE_UNUSED_;                   \
        GTEST_DISALLOW_COPY_AND_ASSIGN_(                                                        \
            GTEST_TEST_CLASS_NAME_(test_suite_name, test_name));                                \
    };                                                                                          \
                                                                                                \
    ::testing::TestInfo *const GTEST_TEST_CLASS_NAME_(test_suite_name, test_name)::test_info_ = \
        ::testing::internal::MakeAndRegisterTestInfo(                                           \
            #test_suite_name, #test_name, nullptr, nullptr,                                     \
            ::testing::internal::CodeLocation(__FILE__, __LINE__), (parent_id),                 \
            ::testing::internal::SuiteApiResolver<                                              \
                parent_class>::GetSetUpCaseOrSuite(),                                           \
            ::testing::internal::SuiteApiResolver<                                              \
                parent_class>::GetTearDownCaseOrSuite(),                                        \
            new ::testing::internal::TestFactoryImpl<GTEST_TEST_CLASS_NAME_(                    \
                test_suite_name, test_name)>);                                                  \
    template <typename FamilyType>                                                              \
    void GTEST_TEST_CLASS_NAME_(test_suite_name, test_name)::testBodyHw()

#define HWTEST_F(test_fixture, test_name)               \
    HWTEST_TEST_(test_fixture, test_name, test_fixture, \
                 ::testing::internal::GetTypeId<test_fixture>())

#define PLATFORM_EXCLUDES_CLASS_NAME(test_suite_name, test_name) \
    PLATFORM_EXCLUDES_##test_suite_name##test_name

// Macros to provide template based testing.
// Test can use FamilyType in the test -- equivalent to SKLFamily
#define HWCMDTEST_TEST_(cmdset_gen_base, test_suite_name, test_name, parent_class, parent_id)             \
    CHECK_TEST_NAME_LENGTH(test_suite_name, test_name)                                                    \
    class PLATFORM_EXCLUDES_CLASS_NAME(test_suite_name, test_name) {                                      \
      public:                                                                                             \
        static std::unique_ptr<std::unordered_set<uint32_t>> &getExcludes() {                             \
            static std::unique_ptr<std::unordered_set<uint32_t>> excludes;                                \
            return excludes;                                                                              \
        }                                                                                                 \
        static void addExclude(uint32_t product) {                                                        \
            auto &excludes = getExcludes();                                                               \
            if (excludes == nullptr) {                                                                    \
                excludes = std::make_unique<std::unordered_set<uint32_t>>();                              \
            }                                                                                             \
            excludes->insert(product);                                                                    \
        }                                                                                                 \
    };                                                                                                    \
                                                                                                          \
    class GTEST_TEST_CLASS_NAME_(test_suite_name, test_name) : public parent_class {                      \
      public:                                                                                             \
        GTEST_TEST_CLASS_NAME_(test_suite_name, test_name)                                                \
        () {}                                                                                             \
                                                                                                          \
      private:                                                                                            \
        template <typename FamilyType>                                                                    \
        void testBodyHw();                                                                                \
                                                                                                          \
        template <typename FamilyType, bool ShouldBeTested = FamilyType::supportsCmdSet(cmdset_gen_base)> \
        auto runCmdTestHwIfSupported() -> typename std::enable_if<ShouldBeTested>::type {                 \
            if (notExcluded()) {                                                                          \
                testBodyHw<FamilyType>();                                                                 \
            }                                                                                             \
        }                                                                                                 \
                                                                                                          \
        bool notExcluded() const {                                                                        \
            using ExcludesT = PLATFORM_EXCLUDES_CLASS_NAME(test_suite_name, test_name);                   \
            auto &excludes = ExcludesT::getExcludes();                                                    \
            if (excludes == nullptr) {                                                                    \
                return true;                                                                              \
            }                                                                                             \
            return excludes->count(::productFamily) == 0;                                                 \
        }                                                                                                 \
                                                                                                          \
        template <typename FamilyType, bool ShouldBeTested = FamilyType::supportsCmdSet(cmdset_gen_base)> \
        auto runCmdTestHwIfSupported() -> typename std::enable_if<false == ShouldBeTested>::type {        \
            /* do nothing */                                                                              \
        }                                                                                                 \
                                                                                                          \
        void TestBody() override {                                                                        \
            switch (::renderCoreFamily) {                                                                 \
            case IGFX_GEN8_CORE:                                                                          \
                BDW_TYPED_CMDTEST_BODY                                                                    \
                break;                                                                                    \
            case IGFX_GEN9_CORE:                                                                          \
                SKL_TYPED_CMDTEST_BODY                                                                    \
                break;                                                                                    \
            case IGFX_GEN10_CORE:                                                                         \
                CNL_TYPED_CMDTEST_BODY                                                                    \
                break;                                                                                    \
            case IGFX_GEN11_CORE:                                                                         \
                ICL_TYPED_CMDTEST_BODY                                                                    \
                break;                                                                                    \
            default:                                                                                      \
                ASSERT_TRUE((false && "Unknown hardware family"));                                        \
                break;                                                                                    \
            }                                                                                             \
        }                                                                                                 \
        void SetUp() override {                                                                           \
            if (notExcluded()) {                                                                          \
                parent_class::SetUp();                                                                    \
            }                                                                                             \
        }                                                                                                 \
        void TearDown() override {                                                                        \
            if (notExcluded()) {                                                                          \
                parent_class::TearDown();                                                                 \
            }                                                                                             \
        }                                                                                                 \
        static ::testing::TestInfo *const test_info_ GTEST_ATTRIBUTE_UNUSED_;                             \
        GTEST_DISALLOW_COPY_AND_ASSIGN_(                                                                  \
            GTEST_TEST_CLASS_NAME_(test_suite_name, test_name));                                          \
    };                                                                                                    \
                                                                                                          \
    ::testing::TestInfo *const GTEST_TEST_CLASS_NAME_(test_suite_name, test_name)::test_info_ =           \
        ::testing::internal::MakeAndRegisterTestInfo(                                                     \
            #test_suite_name, #test_name, nullptr, nullptr,                                               \
            ::testing::internal::CodeLocation(__FILE__, __LINE__), (parent_id),                           \
            ::testing::internal::SuiteApiResolver<                                                        \
                parent_class>::GetSetUpCaseOrSuite(),                                                     \
            ::testing::internal::SuiteApiResolver<                                                        \
                parent_class>::GetTearDownCaseOrSuite(),                                                  \
            new ::testing::internal::TestFactoryImpl<GTEST_TEST_CLASS_NAME_(                              \
                test_suite_name, test_name)>);                                                            \
    template <typename FamilyType>                                                                        \
    void GTEST_TEST_CLASS_NAME_(test_suite_name, test_name)::testBodyHw()

#define HWCMDTEST_EXCLUDE_FAMILY(test_suite_name, test_name, family)                      \
    CHECK_TEST_NAME_LENGTH(test_suite_name, test_name)                                    \
    class PLATFORM_EXCLUDES_CLASS_NAME(test_suite_name, test_name) {                      \
      public:                                                                             \
        static std::unique_ptr<std::unordered_set<uint32_t>> &getExcludes() {             \
            static std::unique_ptr<std::unordered_set<uint32_t>> excludes;                \
            return excludes;                                                              \
        }                                                                                 \
        static void addExclude(uint32_t product) {                                        \
            auto &excludes = getExcludes();                                               \
            if (excludes == nullptr) {                                                    \
                excludes = std::make_unique<std::unordered_set<uint32_t>>();              \
            }                                                                             \
            excludes->insert(product);                                                    \
        }                                                                                 \
    };                                                                                    \
                                                                                          \
    struct test_suite_name##test_name##_PLATFORM_EXCLUDES_EXCLUDE_##family {              \
        test_suite_name##test_name##_PLATFORM_EXCLUDES_EXCLUDE_##family() {               \
            PLATFORM_EXCLUDES_CLASS_NAME(test_suite_name, test_name)::addExclude(family); \
        }                                                                                 \
    } test_suite_name##test_name##_PLATFORM_EXCLUDES_EXCLUDE_##family##_init;

#define HWCMDTEST_F(cmdset_gen_base, test_fixture, test_name)               \
    HWCMDTEST_TEST_(cmdset_gen_base, test_fixture, test_name, test_fixture, \
                    ::testing::internal::GetTypeId<test_fixture>())

#define CALL_IF_MATCH(match_core, match_product, expr)                           \
    auto matchCore = match_core;                                                 \
    auto matchProduct = match_product;                                           \
    if ((::renderCoreFamily == matchCore) &&                                     \
        (IGFX_MAX_PRODUCT == matchProduct || ::productFamily == matchProduct)) { \
        expr;                                                                    \
    }

#define FAMILYTEST_TEST_(test_suite_name, test_name, parent_class, parent_id, match_core, match_product) \
    CHECK_TEST_NAME_LENGTH(test_suite_name, test_name)                                                   \
    class GTEST_TEST_CLASS_NAME_(test_suite_name, test_name) : public parent_class {                     \
      public:                                                                                            \
        GTEST_TEST_CLASS_NAME_(test_suite_name, test_name)                                               \
        () {}                                                                                            \
                                                                                                         \
      private:                                                                                           \
        template <typename FamilyType>                                                                   \
        void testBodyHw();                                                                               \
                                                                                                         \
        void TestBody() override {                                                                       \
            CALL_IF_MATCH(match_core, match_product,                                                     \
                          testBodyHw<typename NEO::GfxFamilyMapper<match_core>::GfxFamily>())            \
        }                                                                                                \
        void SetUp() override {                                                                          \
            CALL_IF_MATCH(match_core, match_product, parent_class::SetUp())                              \
        }                                                                                                \
        void TearDown() override {                                                                       \
            CALL_IF_MATCH(match_core, match_product, parent_class::TearDown())                           \
        }                                                                                                \
        static ::testing::TestInfo *const test_info_ GTEST_ATTRIBUTE_UNUSED_;                            \
        GTEST_DISALLOW_COPY_AND_ASSIGN_(                                                                 \
            GTEST_TEST_CLASS_NAME_(test_suite_name, test_name));                                         \
    };                                                                                                   \
                                                                                                         \
    ::testing::TestInfo *const GTEST_TEST_CLASS_NAME_(test_suite_name, test_name)::test_info_ =          \
        ::testing::internal::MakeAndRegisterTestInfo(                                                    \
            #test_suite_name, #test_name, nullptr, nullptr,                                              \
            ::testing::internal::CodeLocation(__FILE__, __LINE__), (parent_id),                          \
            ::testing::internal::SuiteApiResolver<                                                       \
                parent_class>::GetSetUpCaseOrSuite(),                                                    \
            ::testing::internal::SuiteApiResolver<                                                       \
                parent_class>::GetTearDownCaseOrSuite(),                                                 \
            new ::testing::internal::TestFactoryImpl<GTEST_TEST_CLASS_NAME_(                             \
                test_suite_name, test_name)>);                                                           \
    template <typename FamilyType>                                                                       \
    void GTEST_TEST_CLASS_NAME_(test_suite_name, test_name)::testBodyHw()

// Equivalent Hw specific macro for permuted tests
// Test can use FamilyType in the test -- equivalent to SKLFamily
#define HWTEST_P(test_suite_name, test_name)                                                                                                              \
    CHECK_TEST_NAME_LENGTH(test_suite_name, test_name)                                                                                                    \
    class GTEST_TEST_CLASS_NAME_(test_suite_name, test_name) : public test_suite_name {                                                                   \
      public:                                                                                                                                             \
        GTEST_TEST_CLASS_NAME_(test_suite_name, test_name)                                                                                                \
        () {}                                                                                                                                             \
        template <typename FamilyType>                                                                                                                    \
        void testBodyHw();                                                                                                                                \
                                                                                                                                                          \
        virtual void TestBody() {                                                                                                                         \
            switch (::renderCoreFamily) {                                                                                                                 \
            case IGFX_GEN8_CORE:                                                                                                                          \
                BDW_TYPED_TEST_BODY                                                                                                                       \
                break;                                                                                                                                    \
            case IGFX_GEN9_CORE:                                                                                                                          \
                SKL_TYPED_TEST_BODY                                                                                                                       \
                break;                                                                                                                                    \
            case IGFX_GEN10_CORE:                                                                                                                         \
                CNL_TYPED_TEST_BODY                                                                                                                       \
                break;                                                                                                                                    \
            case IGFX_GEN11_CORE:                                                                                                                         \
                ICL_TYPED_TEST_BODY                                                                                                                       \
                break;                                                                                                                                    \
            default:                                                                                                                                      \
                ASSERT_TRUE((false && "Unknown hardware family"));                                                                                        \
                break;                                                                                                                                    \
            }                                                                                                                                             \
        }                                                                                                                                                 \
                                                                                                                                                          \
      private:                                                                                                                                            \
        static int AddToRegistry() {                                                                                                                      \
            ::testing::UnitTest::GetInstance()->parameterized_test_registry().GetTestCasePatternHolder<test_suite_name>(                                  \
                                                                                 #test_suite_name, ::testing::internal::CodeLocation(__FILE__, __LINE__)) \
                ->AddTestPattern(                                                                                                                         \
                    #test_suite_name,                                                                                                                     \
                    #test_name,                                                                                                                           \
                    new ::testing::internal::TestMetaFactory<                                                                                             \
                        GTEST_TEST_CLASS_NAME_(test_suite_name, test_name)>());                                                                           \
            return 0;                                                                                                                                     \
        }                                                                                                                                                 \
        static int gtest_registering_dummy_;                                                                                                              \
        GTEST_DISALLOW_COPY_AND_ASSIGN_(                                                                                                                  \
            GTEST_TEST_CLASS_NAME_(test_suite_name, test_name));                                                                                          \
    };                                                                                                                                                    \
    int GTEST_TEST_CLASS_NAME_(test_suite_name,                                                                                                           \
                               test_name)::gtest_registering_dummy_ =                                                                                     \
        GTEST_TEST_CLASS_NAME_(test_suite_name, test_name)::AddToRegistry();                                                                              \
    template <typename FamilyType>                                                                                                                        \
    void GTEST_TEST_CLASS_NAME_(test_suite_name, test_name)::testBodyHw()

#define HWCMDTEST_P(cmdset_gen_base, test_suite_name, test_name)                                                                                          \
    CHECK_TEST_NAME_LENGTH(test_suite_name, test_name)                                                                                                    \
    class GTEST_TEST_CLASS_NAME_(test_suite_name, test_name) : public test_suite_name {                                                                   \
      public:                                                                                                                                             \
        GTEST_TEST_CLASS_NAME_(test_suite_name, test_name)                                                                                                \
        () {}                                                                                                                                             \
        template <typename FamilyType>                                                                                                                    \
        void testBodyHw();                                                                                                                                \
                                                                                                                                                          \
        template <typename FamilyType, bool ShouldBeTested = FamilyType::supportsCmdSet(cmdset_gen_base)>                                                 \
        auto runCmdTestHwIfSupported() -> typename std::enable_if<ShouldBeTested>::type {                                                                 \
            testBodyHw<FamilyType>();                                                                                                                     \
        }                                                                                                                                                 \
                                                                                                                                                          \
        template <typename FamilyType, bool ShouldBeTested = FamilyType::supportsCmdSet(cmdset_gen_base)>                                                 \
        auto runCmdTestHwIfSupported() -> typename std::enable_if<false == ShouldBeTested>::type {                                                        \
            /* do nothing */                                                                                                                              \
        }                                                                                                                                                 \
                                                                                                                                                          \
        virtual void TestBody() {                                                                                                                         \
            switch (::renderCoreFamily) {                                                                                                                 \
            case IGFX_GEN8_CORE:                                                                                                                          \
                BDW_TYPED_CMDTEST_BODY                                                                                                                    \
                break;                                                                                                                                    \
            case IGFX_GEN9_CORE:                                                                                                                          \
                SKL_TYPED_CMDTEST_BODY                                                                                                                    \
                break;                                                                                                                                    \
            case IGFX_GEN10_CORE:                                                                                                                         \
                CNL_TYPED_CMDTEST_BODY                                                                                                                    \
                break;                                                                                                                                    \
            case IGFX_GEN11_CORE:                                                                                                                         \
                ICL_TYPED_CMDTEST_BODY                                                                                                                    \
                break;                                                                                                                                    \
            default:                                                                                                                                      \
                ASSERT_TRUE((false && "Unknown hardware family"));                                                                                        \
                break;                                                                                                                                    \
            }                                                                                                                                             \
        }                                                                                                                                                 \
                                                                                                                                                          \
      private:                                                                                                                                            \
        static int AddToRegistry() {                                                                                                                      \
            ::testing::UnitTest::GetInstance()->parameterized_test_registry().GetTestCasePatternHolder<test_suite_name>(                                  \
                                                                                 #test_suite_name, ::testing::internal::CodeLocation(__FILE__, __LINE__)) \
                ->AddTestPattern(                                                                                                                         \
                    #test_suite_name,                                                                                                                     \
                    #test_name,                                                                                                                           \
                    new ::testing::internal::TestMetaFactory<                                                                                             \
                        GTEST_TEST_CLASS_NAME_(test_suite_name, test_name)>());                                                                           \
            return 0;                                                                                                                                     \
        }                                                                                                                                                 \
        static int gtest_registering_dummy_;                                                                                                              \
        GTEST_DISALLOW_COPY_AND_ASSIGN_(                                                                                                                  \
            GTEST_TEST_CLASS_NAME_(test_suite_name, test_name));                                                                                          \
    };                                                                                                                                                    \
    int GTEST_TEST_CLASS_NAME_(test_suite_name,                                                                                                           \
                               test_name)::gtest_registering_dummy_ =                                                                                     \
        GTEST_TEST_CLASS_NAME_(test_suite_name, test_name)::AddToRegistry();                                                                              \
    template <typename FamilyType>                                                                                                                        \
    void GTEST_TEST_CLASS_NAME_(test_suite_name, test_name)::testBodyHw()

#define FAMILYTEST_TEST_P(test_suite_name, test_name, match_core, match_product)                                                                          \
    CHECK_TEST_NAME_LENGTH(test_suite_name, test_name)                                                                                                    \
    class GTEST_TEST_CLASS_NAME_(test_suite_name, test_name) : public test_suite_name {                                                                   \
      public:                                                                                                                                             \
        GTEST_TEST_CLASS_NAME_(test_suite_name, test_name)                                                                                                \
        () {}                                                                                                                                             \
        template <typename FamilyType>                                                                                                                    \
        void testBodyHw();                                                                                                                                \
                                                                                                                                                          \
        void TestBody() override {                                                                                                                        \
            CALL_IF_MATCH(match_core, match_product,                                                                                                      \
                          testBodyHw<typename NEO::GfxFamilyMapper<match_core>::GfxFamily>())                                                             \
        }                                                                                                                                                 \
        void SetUp() override {                                                                                                                           \
            CALL_IF_MATCH(match_core, match_product, test_suite_name::SetUp())                                                                            \
        }                                                                                                                                                 \
        void TearDown() override {                                                                                                                        \
            CALL_IF_MATCH(match_core, match_product, test_suite_name::TearDown())                                                                         \
        }                                                                                                                                                 \
                                                                                                                                                          \
      private:                                                                                                                                            \
        static int AddToRegistry() {                                                                                                                      \
            ::testing::UnitTest::GetInstance()->parameterized_test_registry().GetTestCasePatternHolder<test_suite_name>(                                  \
                                                                                 #test_suite_name, ::testing::internal::CodeLocation(__FILE__, __LINE__)) \
                ->AddTestPattern(                                                                                                                         \
                    #test_suite_name,                                                                                                                     \
                    #test_name,                                                                                                                           \
                    new ::testing::internal::TestMetaFactory<                                                                                             \
                        GTEST_TEST_CLASS_NAME_(test_suite_name, test_name)>());                                                                           \
            return 0;                                                                                                                                     \
        }                                                                                                                                                 \
        static int gtest_registering_dummy_;                                                                                                              \
        GTEST_DISALLOW_COPY_AND_ASSIGN_(                                                                                                                  \
            GTEST_TEST_CLASS_NAME_(test_suite_name, test_name));                                                                                          \
    };                                                                                                                                                    \
    int GTEST_TEST_CLASS_NAME_(test_suite_name,                                                                                                           \
                               test_name)::gtest_registering_dummy_ =                                                                                     \
        GTEST_TEST_CLASS_NAME_(test_suite_name, test_name)::AddToRegistry();                                                                              \
    template <typename FamilyType>                                                                                                                        \
    void GTEST_TEST_CLASS_NAME_(test_suite_name, test_name)::testBodyHw()

#ifdef TESTS_GEN8
#define GEN8TEST_F(test_fixture, test_name)                          \
    FAMILYTEST_TEST_(test_fixture, test_name, test_fixture,          \
                     ::testing::internal::GetTypeId<test_fixture>(), \
                     IGFX_GEN8_CORE, IGFX_MAX_PRODUCT)
#define GEN8TEST_P(test_suite_name, test_name)    \
    FAMILYTEST_TEST_P(test_suite_name, test_name, \
                      IGFX_GEN8_CORE,             \
                      IGFX_MAX_PRODUCT)
#endif
#ifdef TESTS_GEN9
#define GEN9TEST_F(test_fixture, test_name)                          \
    FAMILYTEST_TEST_(test_fixture, test_name, test_fixture,          \
                     ::testing::internal::GetTypeId<test_fixture>(), \
                     IGFX_GEN9_CORE, IGFX_MAX_PRODUCT)
#define GEN9TEST_P(test_suite_name, test_name)    \
    FAMILYTEST_TEST_P(test_suite_name, test_name, \
                      IGFX_GEN9_CORE,             \
                      IGFX_MAX_PRODUCT)
#endif
#ifdef TESTS_GEN10
#define GEN10TEST_F(test_fixture, test_name)                         \
    FAMILYTEST_TEST_(test_fixture, test_name, test_fixture,          \
                     ::testing::internal::GetTypeId<test_fixture>(), \
                     IGFX_GEN10_CORE, IGFX_MAX_PRODUCT)
#define GEN10TEST_P(test_suite_name, test_name)   \
    FAMILYTEST_TEST_P(test_suite_name, test_name, \
                      IGFX_GEN10_CORE,            \
                      IGFX_MAX_PRODUCT)
#endif
#ifdef TESTS_GEN11
#define GEN11TEST_F(test_fixture, test_name)                         \
    FAMILYTEST_TEST_(test_fixture, test_name, test_fixture,          \
                     ::testing::internal::GetTypeId<test_fixture>(), \
                     IGFX_GEN11_CORE, IGFX_MAX_PRODUCT)
#define GEN11TEST_P(test_suite_name, test_name)   \
    FAMILYTEST_TEST_P(test_suite_name, test_name, \
                      IGFX_GEN11_CORE,            \
                      IGFX_MAX_PRODUCT)
#endif

#ifdef TESTS_GEN8
#define BDWTEST_F(test_fixture, test_name)                           \
    FAMILYTEST_TEST_(test_fixture, test_name, test_fixture,          \
                     ::testing::internal::GetTypeId<test_fixture>(), \
                     IGFX_GEN8_CORE, IGFX_BROADWELL)
#define BDWTEST_P(test_suite_name, test_name)     \
    FAMILYTEST_TEST_P(test_suite_name, test_name, \
                      IGFX_GEN8_CORE,             \
                      IGFX_BROADWELL)
#endif
#ifdef TESTS_SKL
#define SKLTEST_F(test_fixture, test_name)                           \
    FAMILYTEST_TEST_(test_fixture, test_name, test_fixture,          \
                     ::testing::internal::GetTypeId<test_fixture>(), \
                     IGFX_GEN9_CORE, IGFX_SKYLAKE)
#define SKLTEST_P(test_suite_name, test_name)     \
    FAMILYTEST_TEST_P(test_suite_name, test_name, \
                      IGFX_GEN9_CORE,             \
                      IGFX_SKYLAKE)
#endif
#ifdef TESTS_KBL
#define KBLTEST_F(test_fixture, test_name)                           \
    FAMILYTEST_TEST_(test_fixture, test_name, test_fixture,          \
                     ::testing::internal::GetTypeId<test_fixture>(), \
                     IGFX_GEN9_CORE, IGFX_KABYLAKE)
#define KBLTEST_P(test_suite_name, test_name)     \
    FAMILYTEST_TEST_P(test_suite_name, test_name, \
                      IGFX_GEN9_CORE,             \
                      IGFX_KABYLAKE)
#endif
#ifdef TESTS_GLK
#define GLKTEST_F(test_fixture, test_name)                           \
    FAMILYTEST_TEST_(test_fixture, test_name, test_fixture,          \
                     ::testing::internal::GetTypeId<test_fixture>(), \
                     IGFX_GEN9_CORE, IGFX_GEMINILAKE)
#define GLKTEST_P(test_suite_name, test_name)     \
    FAMILYTEST_TEST_P(test_suite_name, test_name, \
                      IGFX_GEN9_CORE,             \
                      IGFX_GEMINILAKE)
#endif
#ifdef TESTS_BXT
#define BXTTEST_F(test_fixture, test_name)                           \
    FAMILYTEST_TEST_(test_fixture, test_name, test_fixture,          \
                     ::testing::internal::GetTypeId<test_fixture>(), \
                     IGFX_GEN9_CORE, IGFX_BROXTON)
#define BXTTEST_P(test_suite_name, test_name)     \
    FAMILYTEST_TEST_P(test_suite_name, test_name, \
                      IGFX_GEN9_CORE,             \
                      IGFX_BROXTON)
#endif
#ifdef TESTS_CFL
#define CFLTEST_F(test_fixture, test_name)                           \
    FAMILYTEST_TEST_(test_fixture, test_name, test_fixture,          \
                     ::testing::internal::GetTypeId<test_fixture>(), \
                     IGFX_GEN9_CORE, IGFX_COFFEELAKE)
#define CFLTEST_P(test_suite_name, test_name)     \
    FAMILYTEST_TEST_P(test_suite_name, test_name, \
                      IGFX_GEN9_CORE,             \
                      IGFX_COFFEELAKE)
#endif
#ifdef TESTS_GEN10
#define CNLTEST_F(test_fixture, test_name)                           \
    FAMILYTEST_TEST_(test_fixture, test_name, test_fixture,          \
                     ::testing::internal::GetTypeId<test_fixture>(), \
                     IGFX_GEN10_CORE, IGFX_CANNONLAKE)
#define CNLTEST_P(test_suite_name, test_name)     \
    FAMILYTEST_TEST_P(test_suite_name, test_name, \
                      IGFX_GEN10_CORE,            \
                      IGFX_CANNONLAKE)
#endif
#ifdef TESTS_GEN11
#define ICLLPTEST_F(test_fixture, test_name)                         \
    FAMILYTEST_TEST_(test_fixture, test_name, test_fixture,          \
                     ::testing::internal::GetTypeId<test_fixture>(), \
                     IGFX_GEN11_CORE, IGFX_ICELAKE_LP)
#define ICLLPTEST_P(test_suite_name, test_name)   \
    FAMILYTEST_TEST_P(test_suite_name, test_name, \
                      IGFX_GEN11_CORE,            \
                      IGFX_ICELAKE_LP)
#define LKFTEST_F(test_fixture, test_name)                           \
    FAMILYTEST_TEST_(test_fixture, test_name, test_fixture,          \
                     ::testing::internal::GetTypeId<test_fixture>(), \
                     IGFX_GEN11_CORE, IGFX_LAKEFIELD)
#define LKFTEST_P(test_suite_name, test_name)     \
    FAMILYTEST_TEST_P(test_suite_name, test_name, \
                      IGFX_GEN11_CORE,            \
                      IGFX_LAKEFIELD)
#endif
#define HWTEST_TYPED_TEST(CaseName, TestName)                                                                  \
    CHECK_TEST_NAME_LENGTH(CaseName, TestName)                                                                 \
    template <typename gtest_TypeParam_>                                                                       \
    class GTEST_TEST_CLASS_NAME_(CaseName, TestName) : public CaseName<gtest_TypeParam_> {                     \
      private:                                                                                                 \
        typedef CaseName<gtest_TypeParam_> TestFixture;                                                        \
        typedef gtest_TypeParam_ TypeParam;                                                                    \
        template <typename FamilyType>                                                                         \
        void testBodyHw();                                                                                     \
                                                                                                               \
        void TestBody() override {                                                                             \
            switch (::renderCoreFamily) {                                                                      \
            case IGFX_GEN8_CORE:                                                                               \
                BDW_TYPED_TEST_BODY                                                                            \
                break;                                                                                         \
            case IGFX_GEN9_CORE:                                                                               \
                SKL_TYPED_TEST_BODY                                                                            \
                break;                                                                                         \
            case IGFX_GEN10_CORE:                                                                              \
                CNL_TYPED_TEST_BODY                                                                            \
                break;                                                                                         \
            case IGFX_GEN11_CORE:                                                                              \
                ICL_TYPED_TEST_BODY                                                                            \
                break;                                                                                         \
            default:                                                                                           \
                ASSERT_TRUE((false && "Unknown hardware family"));                                             \
                break;                                                                                         \
            }                                                                                                  \
        }                                                                                                      \
    };                                                                                                         \
    bool gtest_##CaseName##_##TestName##_registered_ GTEST_ATTRIBUTE_UNUSED_ =                                 \
        ::testing::internal::TypeParameterizedTest<                                                            \
            CaseName,                                                                                          \
            ::testing::internal::TemplateSel<                                                                  \
                GTEST_TEST_CLASS_NAME_(CaseName, TestName)>,                                                   \
            GTEST_TYPE_PARAMS_(CaseName)>::Register("", ::testing::internal::CodeLocation(__FILE__, __LINE__), \
                                                    #CaseName, #TestName, 0);                                  \
    template <typename gtest_TypeParam_>                                                                       \
    template <typename FamilyType>                                                                             \
    void GTEST_TEST_CLASS_NAME_(CaseName, TestName)<gtest_TypeParam_>::testBodyHw()

template <typename Fixture>
struct Test
    : public Fixture,
      public ::testing::Test {

    void SetUp() override {
        Fixture::SetUp();
    }

    void TearDown() override {
        Fixture::TearDown();
    }
};
