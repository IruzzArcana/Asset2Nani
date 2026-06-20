#include "JSONValidator.hpp"

void JSONValidator::Validate()
{
    for (const auto &field : m_fields)
    {
        std::string currentPath = m_path.empty() ? field.name : m_path + "/" + field.name;

        if (!m_data.contains(field.name))
            throw std::runtime_error(std::format("Missing {}", currentPath));

        const nlohmann::json &value = m_data[field.name];

        if (field.isArray)
        {
            if (!value.is_array())
                throw std::runtime_error(std::format("{} must be an array", currentPath));

            for (size_t i = 0; i < value.size(); ++i)
            {
                JSONValidator arrayItemValidator(value[i], field.children, std::format("{}/[{}]", currentPath, i));
                arrayItemValidator.Validate();
            }
        }
        else if (!field.children.empty())
        {
            JSONValidator nestedObjectValidator(value, field.children, currentPath);
            nestedObjectValidator.Validate();
        }
    }
}