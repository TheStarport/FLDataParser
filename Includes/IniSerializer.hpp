#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Structures/INI/IniSection.hpp"

#include <refl.hpp>

namespace Ini
{
    struct IniFile
    {};

    struct Subsection
    {};

    template <typename>
    struct IsVector : std::false_type
    {};

    template <typename T, typename A>
    struct IsVector<std::vector<T, A>> : std::true_type
    {};

    template <typename T>
    constexpr auto IsEnum = std::is_enum_v<T>;
    template <typename T>
    constexpr auto IsBool = std::is_same_v<T, bool>;
    template <typename T>
    constexpr auto IsInt = std::is_same_v<T, int> || std::is_same_v<T, unsigned> || IsEnum<T>;
    template <typename T>
    constexpr auto IsBigInt = std::is_same_v<T, long long> || std::is_same_v<T, unsigned long long>;
    template <typename T>
    constexpr auto IsFloat = std::is_same_v<T, float>;
    template <typename T>
    constexpr auto IsString = std::is_same_v<T, std::string>;
    template <typename T>
    constexpr auto IsWString = std::is_same_v<T, std::wstring>;
    template <typename T>
    constexpr auto IsIniFile = std::is_base_of_v<IniFile, T>;
    template <typename T>
    constexpr auto IsSection = std::is_base_of_v<Subsection, T>;
    template <typename T>
    constexpr auto IsAnyValidType = IsBool<T> || IsInt<T> || IsBigInt<T> || IsFloat<T> || IsString<T> || IsWString<T> || IsVector<T>::value;
    template <typename T, typename TT>
    constexpr auto IsVectorOfType = std::is_same_v<T, std::vector<TT>>;

    struct SectionName : refl::attr::usage::field
    {
            std::string name;
            SectionName() = delete;
            explicit constexpr SectionName(std::string name) : name(name) {}
    };

    struct IniKey : refl::attr::usage::field
    {
            std::string name;
            IniKey() = delete;
            explicit constexpr IniKey(std::string name) : name(name) {}
    };

    class IniSerializer
    {
            template <class... Ts>
            struct overload : Ts...
            {
                    using Ts::operator()...;
            };

            template <typename T>
            static T DeserializeVector(IniEntry& entry)
            {}

            template <typename T>
            static T DeserializeKey(IniEntry& entry)
            {
                static_assert(!IsSection<T>, "Sections are not valid subobjects");
                static_assert(
                    IsAnyValidType<T>,
                    "Provided value is not a valid type. Sections can only contain, strings, ints, floats, bools, or vectors of the aforementioned types.");

                // No value, no processing
                if (entry.begin() == entry.end())
                {
                    return T();
                }

                if constexpr (IsVector<T>::value)
                {
                    return DeserializeVector<T>(entry);
                }

                auto& variant = *entry.begin();
                auto result = std::visit(
                    overload{
                        [](bool& b) { return b; },
                        [](int& i) { return i; },
                        [](float& f) { return f; },
                        [](std::string_view& s) { return s; },
                        [](std::pair<std::string_view, std::string_view>& s) { return s; },
                    },
                    variant);

                if constexpr (IsString<T>)
                {
                    return *entry.begin();
                }

                return T();
            }

        public:
            template <typename T>
                requires IsIniFile<T>
            static T Deserialize(const std::vector<IniSection>& sections)
            {
                T newObj;
                using namespace refl;
                constexpr auto type = reflect<T>();

                util::for_each(type.members,
                               [&](const auto member)
                               {
                                   if constexpr (!member.is_writable || member.is_static || !descriptor::has_attribute<SectionName>(member))
                                   {
                                       return;
                                   }

                                   const auto& nameAttr = descriptor::get_attribute<SectionName>(member);

                                   using DeclType = std::remove_reference_t<decltype(member(newObj))>;

                                   if constexpr (!IsSection<DeclType>)
                                   {
                                       return;
                                   }

                                   auto& section = std::ranges::find_if(sections, [&](const IniSection& sec) { return sec.GetName() == nameAttr.name; });

                                   if (section == sections.end())
                                   {
                                       return;
                                   }

                                   // Pointer to section instance
                                   void* ptr = reinterpret_cast<void*>(std::addressof(newObj.*member.pointer));

                                   auto subType = reflect<DeclType>();

                                   DeclType completedSection;

                                   util::for_each(type.members,
                                                  [&](const auto typeEntry)
                                                  {
                                                      if constexpr (!typeEntry.is_writable || typeEntry.is_static ||
                                                                    !descriptor::has_attribute<IniKey>(typeEntry))
                                                      {
                                                          return;
                                                      }

                                                      const auto& keyAttr = descriptor::get_attribute<IniKey>(typeEntry);
                                                      auto& entry = std::ranges::find_if(section, [&](const IniKey& k) { return k.name == keyAttr.name; });

                                                      if (entry == section->end())
                                                      {
                                                          return;
                                                      }

                                                      using KeyType = std::remove_reference_t<decltype(typeEntry(completedSection))>;
                                                      typeEntry(completedSection) = DeserializeKey<KeyType>(entry);
                                                  });

                                   *static_cast<DeclType*>(ptr) = completedSection;
                               });
                return newObj;
            }
    };

} // namespace Ini

#define Section(prop, name) field(prop, Ini::SectionName{ name });
#define Key(prop, key)      field(prop, Ini::IniKey{ key })
