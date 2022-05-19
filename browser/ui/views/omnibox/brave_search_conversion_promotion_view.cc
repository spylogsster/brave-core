/* Copyright (c) 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/browser/ui/views/omnibox/brave_search_conversion_promotion_view.h"

#include <utility>

#include "base/logging.h"
#include "brave/browser/themes/theme_properties.h"
#include "brave/browser/ui/views/omnibox/brave_omnibox_result_view.h"
#include "brave/components/l10n/common/locale_util.h"
#include "brave/grit/brave_generated_resources.h"
#include "brave/grit/brave_theme_resources.h"
#include "cc/paint/paint_flags.h"
#include "chrome/browser/ui/layout_constants.h"
#include "chrome/browser/ui/omnibox/omnibox_theme.h"
#include "chrome/browser/ui/views/chrome_typography.h"
#include "components/strings/grit/components_strings.h"
#include "components/vector_icons/vector_icons.h"
#include "third_party/skia/include/core/SkPath.h"
#include "ui/accessibility/ax_enums.mojom.h"
#include "ui/accessibility/ax_node_data.h"
#include "ui/base/l10n/l10n_util.h"
#include "ui/base/metadata/metadata_impl_macros.h"
#include "ui/base/models/image_model.h"
#include "ui/base/resource/resource_bundle.h"
#include "ui/base/theme_provider.h"
#include "ui/gfx/canvas.h"
#include "ui/gfx/geometry/insets.h"
#include "ui/gfx/geometry/skia_conversions.h"
#include "ui/gfx/range/range.h"
#include "ui/gfx/skia_paint_util.h"
#include "ui/views/background.h"
#include "ui/views/cascading_property.h"
#include "ui/views/controls/button/image_button.h"
#include "ui/views/controls/button/image_button_factory.h"
#include "ui/views/controls/button/label_button.h"
#include "ui/views/controls/button/md_text_button.h"
#include "ui/views/controls/focus_ring.h"
#include "ui/views/controls/highlight_path_generator.h"
#include "ui/views/controls/image_view.h"
#include "ui/views/controls/label.h"
#include "ui/views/layout/box_layout.h"
#include "ui/views/layout/fill_layout.h"
#include "ui/views/layout/flex_layout.h"

using brave_search_conversion::ConversionType;

namespace {

constexpr int kBannerTypeMargin = 12;
// Use small margin because omnibox popup has its own bottom padding.
constexpr int kBannerTypeMarginBottom = 4;
constexpr int kBannerTypeRadius = 8;

gfx::FontList GetFont(int font_size, gfx::Font::Weight weight) {
  gfx::FontList font_list;
  return font_list.DeriveWithSizeDelta(font_size - font_list.GetFontSize())
      .DeriveWithWeight(weight);
}

// Draw graphic over gradient background for banner type.
class HorizontalGradientBackground : public views::Background {
 public:
  using Background::Background;
  ~HorizontalGradientBackground() override = default;
  HorizontalGradientBackground(const HorizontalGradientBackground&) = delete;
  HorizontalGradientBackground& operator=(const HorizontalGradientBackground&) =
      delete;

  void Paint(gfx::Canvas* canvas, views::View* view) const override {
    // Fill with base color first.
    canvas->DrawColor(GetOmniboxColor(view->GetThemeProvider(),
                                      OmniboxPart::RESULTS_BACKGROUND));

    SkColor from_color = gfx::kPlaceholderColor;
    SkColor to_color = gfx::kPlaceholderColor;
    if (auto* tp = view->GetThemeProvider()) {
      from_color = tp->GetColor(
          BraveThemeProperties::
              COLOR_SEARCH_CONVERSION_BANNER_TYPE_BACKGROUND_GRADIENT_FROM);
      to_color = tp->GetColor(
          BraveThemeProperties::
              COLOR_SEARCH_CONVERSION_BANNER_TYPE_BACKGROUND_GRADIENT_TO);
    }

    cc::PaintFlags flags;
    SkPoint points[2] = {SkPoint::Make(0, 0), SkPoint::Make(view->width(), 0)};
    SkColor colors[2] = {from_color, to_color};
    SkScalar positions[2] = {0.196f, 1.f};
    flags.setShader(cc::PaintShader::MakeLinearGradient(
        points, colors, positions, 2, SkTileMode::kClamp));
    flags.setStyle(cc::PaintFlags::kFill_Style);
    gfx::RectF bounds(view->GetLocalBounds());
    canvas->DrawRect(bounds, flags);

    auto& bundle = ui::ResourceBundle::GetSharedInstance();
    const auto* graphic = bundle.GetImageSkiaNamed(
        ui::NativeTheme::GetInstanceForNativeUi()->ShouldUseDarkColors()
            ? IDR_BRAVE_SEARCH_CONVERSION_BANNER_GRAPHIC_DARK
            : IDR_BRAVE_SEARCH_CONVERSION_BANNER_GRAPHIC);
    canvas->DrawImageInt(*graphic, bounds.width() - graphic->width(), 0);
  }
};

class TryButton : public views::MdTextButton {
 public:
  METADATA_HEADER(TryButton);
  using MdTextButton::MdTextButton;
  TryButton(const TryButton&) = delete;
  TryButton& operator=(const TryButton&) = delete;

  void SetFontSize(int size) {
    label()->SetFontList(GetFont(size, gfx::Font::Weight::SEMIBOLD));
  }
};

BEGIN_METADATA(TryButton, views::MdTextButton)
END_METADATA

class BannerTypeDismissButton : public views::LabelButton {
 public:
  METADATA_HEADER(BannerTypeDismissButton);
  using LabelButton::LabelButton;
  BannerTypeDismissButton(const BannerTypeDismissButton&) = delete;
  BannerTypeDismissButton& operator=(const BannerTypeDismissButton&) = delete;

  void SetFontSize(int size) {
    label()->SetFontList(GetFont(size, gfx::Font::Weight::SEMIBOLD));
  }
};

BEGIN_METADATA(BannerTypeDismissButton, views::LabelButton)
END_METADATA

class ButtonTypeDismissButton : public views::ImageButton {
 public:
  METADATA_HEADER(ButtonTypeDismissButton);
  explicit ButtonTypeDismissButton(PressedCallback callback = PressedCallback())
      : ImageButton(std::move(callback)) {
    views::ConfigureVectorImageButton(this);
  }
  ButtonTypeDismissButton(const ButtonTypeDismissButton&) = delete;
  ButtonTypeDismissButton& operator=(const ButtonTypeDismissButton&) = delete;

  void GetAccessibleNodeData(ui::AXNodeData* node_data) override {
    node_data->SetName(brave_l10n::GetLocalizedResourceUTF16String(
        IDS_ACC_BRAVE_SEARCH_CONVERSION_DISMISS_BUTTON));
    // Although this appears visually as a button, expose as a list box option
    // so that it matches the other options within its list box container.
    node_data->role = ax::mojom::Role::kListBoxOption;
  }
};

BEGIN_METADATA(ButtonTypeDismissButton, views::ImageButton)
END_METADATA

// For rounded.
class BannerTypeContainer : public views::View {
 public:
  METADATA_HEADER(BannerTypeContainer);
  using View::View;
  BannerTypeContainer(const BannerTypeContainer&) = delete;
  BannerTypeContainer& operator=(const BannerTypeContainer&) = delete;

  // views::View overrides:
  void OnPaint(gfx::Canvas* canvas) override {
    SkPath mask;
    mask.addRoundRect(gfx::RectToSkRect(GetLocalBounds()), kBannerTypeRadius,
                      kBannerTypeRadius);
    canvas->ClipPath(mask, true);

    View::OnPaint(canvas);
  }
};

BEGIN_METADATA(BannerTypeContainer, views::View)
END_METADATA

////////////////////////////////////////////////////////////////////////////////
// OmniboxResultSelectionIndicator

class BraveOmniboxResultSelectionIndicator : public views::View {
 public:
  METADATA_HEADER(BraveOmniboxResultSelectionIndicator);

  static constexpr int kStrokeThickness = 3;

  explicit BraveOmniboxResultSelectionIndicator(
      BraveSearchConversionPromotionView* parent_view)
      : parent_view_(parent_view) {
    SetPreferredSize({kStrokeThickness, 0});
  }

  // views::View:
  void OnPaint(gfx::Canvas* canvas) override {
    SkPath path = GetPath();
    cc::PaintFlags flags;
    flags.setAntiAlias(true);
    flags.setColor(color_);
    flags.setStyle(cc::PaintFlags::kFill_Style);
    canvas->DrawPath(path, flags);
  }

  // views::View:
  void OnThemeChanged() override {
    views::View::OnThemeChanged();

    color_ = views::GetCascadingAccentColor(parent_view_);
  }

 private:
  SkColor color_;

  // Pointer to the parent view.
  const raw_ptr<BraveSearchConversionPromotionView> parent_view_;

  // The focus bar is a straight vertical line with half-rounded endcaps. Since
  // this geometry is nontrivial to represent using primitives, it's instead
  // represented using a fill path. This matches the style and implementation
  // used in Tab Groups.
  SkPath GetPath() const {
    SkPath path;

    path.moveTo(0, 0);
    path.arcTo(kStrokeThickness, kStrokeThickness, 0, SkPath::kSmall_ArcSize,
               SkPathDirection::kCW, kStrokeThickness, kStrokeThickness);
    path.lineTo(kStrokeThickness, height() - kStrokeThickness);
    path.arcTo(kStrokeThickness, kStrokeThickness, 0, SkPath::kSmall_ArcSize,
               SkPathDirection::kCW, 0, height());
    path.close();

    return path;
  }
};

BEGIN_METADATA(BraveOmniboxResultSelectionIndicator, views::View)
END_METADATA

}  // namespace

BraveSearchConversionPromotionView::BraveSearchConversionPromotionView(
    BraveOmniboxResultView* result_view)
    : result_view_(result_view),
      mouse_enter_exit_handler_(base::BindRepeating(
          &BraveSearchConversionPromotionView::UpdateHoverState,
          base::Unretained(this))) {
  SetLayoutManager(std::make_unique<views::FlexLayout>());
}

BraveSearchConversionPromotionView::~BraveSearchConversionPromotionView() =
    default;

void BraveSearchConversionPromotionView::ResetChildrenVisibility() {
  // Reset all children visibility and configure later based on type.
  if (button_type_container_)
    button_type_container_->SetVisible(false);

  if (banner_type_container_)
    banner_type_container_->SetVisible(false);
}

void BraveSearchConversionPromotionView::SetTypeAndInput(
    brave_search_conversion::ConversionType type,
    const std::u16string& input) {
  DCHECK_NE(ConversionType::kNone, type);

  type_ = type;
  input_ = input;
  selected_ = false;

  ResetChildrenVisibility();

  if (type_ == ConversionType::kButton) {
    ConfigureForButtonType();
  } else {
    ConfigureForBannerType();
  }

  UpdateState();
}

void BraveSearchConversionPromotionView::OnSelectionStateChanged(
    bool selected) {
  selected_ = selected;
  UpdateState();
}

void BraveSearchConversionPromotionView::UpdateState() {
  if (type_ == ConversionType::kButton) {
    UpdateButtonTypeState();
  } else {
    UpdateBannerTypeState();
  }
}

void BraveSearchConversionPromotionView::OpenMatch() {
  result_view_->OpenMatch();
}

void BraveSearchConversionPromotionView::Dismiss() {
  result_view_->Dismiss();
}

void BraveSearchConversionPromotionView::UpdateButtonTypeState() {
  if (!button_type_container_)
    return;

  button_type_container_->SetVisible(true);
  button_type_selection_indicator_->SetVisible(selected_);
  button_type_contents_input_->SetText(input_);
  if (auto* tp = GetThemeProvider()) {
    auto desc_color_id =
        BraveThemeProperties::COLOR_SEARCH_CONVERSION_BUTTON_TYPE_DESC_NORMAL;
    if (IsMouseHovered()) {
      desc_color_id = BraveThemeProperties::
          COLOR_SEARCH_CONVERSION_BUTTON_TYPE_DESC_HOVERED;
    }
    button_type_description_->SetEnabledColor(tp->GetColor(desc_color_id));
    append_for_input_->SetEnabledColor(
        tp->GetColor(BraveThemeProperties::
                         COLOR_SEARCH_CONVERSION_BUTTON_TYPE_INPUT_APPEND));
  }

  SetBackground(GetButtonTypeBackground());
}

void BraveSearchConversionPromotionView::UpdateBannerTypeState() {
  if (!banner_type_container_)
    return;

  banner_type_container_->SetVisible(true);
  SkColor desc_color = gfx::kPlaceholderColor;
  SkColor border_color = gfx::kPlaceholderColor;
  if (auto* tp = GetThemeProvider()) {
    desc_color = tp->GetColor(
        BraveThemeProperties::COLOR_SEARCH_CONVERSION_BANNER_TYPE_DESC_TEXT);
    border_color =
        tp->GetColor(BraveThemeProperties::
                         COLOR_SEARCH_CONVERSION_BANNER_TYPE_BACKGROUND_BORDER);
  }
  banner_type_container_->SetBorder(
      views::CreateRoundedRectBorder(1, kBannerTypeRadius, border_color));
  banner_type_description_->SetEnabledColor(desc_color);

  SetBackground(views::CreateSolidBackground(
      GetOmniboxColor(GetThemeProvider(), OmniboxPart::RESULTS_BACKGROUND)));
  banner_type_container_->SetBackground(
      std::make_unique<HorizontalGradientBackground>());
}

void BraveSearchConversionPromotionView::ConfigureForButtonType() {
  if (button_type_container_)
    return;

  button_type_container_ = AddChildView(std::make_unique<views::View>());
  button_type_container_->SetProperty(
      views::kFlexBehaviorKey,
      views::FlexSpecification(views::MinimumFlexSizeRule::kScaleToZero,
                               views::MaximumFlexSizeRule::kUnbounded));

  button_type_container_
      ->SetLayoutManager(std::make_unique<views::FlexLayout>())
      ->SetOrientation(views::LayoutOrientation::kHorizontal);

  auto* selection_container =
      button_type_container_->AddChildView(std::make_unique<views::View>());
  selection_container->SetPreferredSize(gfx::Size(3, height()));
  selection_container->SetLayoutManager(std::make_unique<views::FillLayout>());
  button_type_selection_indicator_ = selection_container->AddChildView(
      std::make_unique<BraveOmniboxResultSelectionIndicator>(this));

  auto& bundle = ui::ResourceBundle::GetSharedInstance();
  auto* icon_view =
      button_type_container_->AddChildView(std::make_unique<views::ImageView>(
          ui::ImageModel::FromImageSkia(*bundle.GetImageSkiaNamed(
              IDR_BRAVE_SEARCH_LOGO_IN_SEARCH_PROMOTION))));
  icon_view->SetProperty(views::kMarginsKey, gfx::Insets::TLBR(14, 12, 36, 8));

  auto* contents_container =
      button_type_container_->AddChildView(std::make_unique<views::View>());
  contents_container->SetProperty(views::kMarginsKey,
                                  gfx::Insets::TLBR(12, 12, 12, 0));
  contents_container->SetLayoutManager(std::make_unique<views::BoxLayout>(
      views::BoxLayout::Orientation::kVertical, gfx::Insets(), 2));

  auto* input_container =
      contents_container->AddChildView(std::make_unique<views::View>());
  input_container->SetLayoutManager(std::make_unique<views::BoxLayout>(
      views::BoxLayout::Orientation::kHorizontal, gfx::Insets(), 8));

  // Use 14px font size for input text.
  views::Label::CustomFont emphasized_font = {
      GetFont(14, gfx::Font::Weight::SEMIBOLD)};
  button_type_contents_input_ = input_container->AddChildView(
      std::make_unique<views::Label>(input_, emphasized_font));
  button_type_contents_input_->SetAutoColorReadabilityEnabled(false);
  button_type_contents_input_->SetHorizontalAlignment(gfx::ALIGN_LEFT);

  views::Label::CustomFont normal_font = {
      GetFont(14, gfx::Font::Weight::NORMAL)};
  append_for_input_ =
      input_container->AddChildView(std::make_unique<views::Label>(
          brave_l10n::GetLocalizedResourceUTF16String(
              IDS_BRAVE_SEARCH_CONVERSION_INPUT_APPEND_LABEL),
          normal_font));
  append_for_input_->SetAutoColorReadabilityEnabled(false);
  append_for_input_->SetHorizontalAlignment(gfx::ALIGN_LEFT);

  // Configure description
  const std::u16string first_part = brave_l10n::GetLocalizedResourceUTF16String(
      IDS_BRAVE_SEARCH_CONVERSION_PROMOTION_DESC_LABEL_FIRST_PART);
  const std::u16string desc_label = brave_l10n::GetLocalizedResourceUTF16String(
      IDS_BRAVE_SEARCH_CONVERSION_PROMOTION_DESC_LABEL);

  button_type_description_ = contents_container->AddChildView(
      std::make_unique<views::Label>(desc_label, normal_font));
  button_type_description_->SetAutoColorReadabilityEnabled(false);
  button_type_description_->SetHorizontalAlignment(gfx::ALIGN_LEFT);
  button_type_description_->SetTextStyleRange(
      ChromeTextStyle::STYLE_EMPHASIZED, gfx::Range(0, first_part.length()));

  contents_container->SetProperty(
      views::kFlexBehaviorKey,
      views::FlexSpecification(views::MinimumFlexSizeRule::kScaleToZero,
                               views::MaximumFlexSizeRule::kUnbounded)
          .WithOrder(2));

  auto* try_button = button_type_container_->AddChildView(
      std::make_unique<TryButton>(views::Button::PressedCallback(
          base::BindRepeating(&BraveSearchConversionPromotionView::OpenMatch,
                              base::Unretained(this)))));
  try_button->SetProperty(views::kMarginsKey, gfx::Insets::TLBR(17, 0, 17, 0));
  try_button->SetProminent(true);
  try_button->SetText(brave_l10n::GetLocalizedResourceUTF16String(
      IDS_BRAVE_SEARCH_CONVERSION_TRY_BUTTON_LABEL));
  try_button->SetFontSize(12);

  auto* dismiss_button = button_type_container_->AddChildView(
      std::make_unique<ButtonTypeDismissButton>(views::Button::PressedCallback(
          base::BindRepeating(&BraveSearchConversionPromotionView::Dismiss,
                              base::Unretained(this)))));
  views::SetImageFromVectorIconWithColor(
      dismiss_button, vector_icons::kCloseRoundedIcon,
      GetLayoutConstant(LOCATION_BAR_ICON_SIZE),
      SkColorSetRGB(0x6B, 0x70, 0x84), gfx::kPlaceholderColor);
  views::InstallCircleHighlightPathGenerator(dismiss_button);
  views::FocusRing::Install(dismiss_button);
  dismiss_button->SetProperty(views::kMarginsKey, gfx::Insets::VH(25, 8));
  dismiss_button->SetTooltipText(brave_l10n::GetLocalizedResourceUTF16String(
      IDS_BRAVE_SEARCH_CONVERSION_DISMISS_BUTTON_TOOLTIP));

  mouse_enter_exit_handler_.ObserveMouseEnterExitOn(button_type_container_);
}

void BraveSearchConversionPromotionView::ConfigureForBannerType() {
  if (banner_type_container_)
    return;

  banner_type_container_ =
      AddChildView(std::make_unique<BannerTypeContainer>());
  banner_type_container_->SetProperty(
      views::kMarginsKey,
      gfx::Insets::TLBR(kBannerTypeMargin, kBannerTypeMargin,
                        kBannerTypeMarginBottom, kBannerTypeMargin));
  banner_type_container_->SetProperty(
      views::kFlexBehaviorKey,
      views::FlexSpecification(views::MinimumFlexSizeRule::kScaleToZero,
                               views::MaximumFlexSizeRule::kUnbounded));

  banner_type_container_
      ->SetLayoutManager(std::make_unique<views::FlexLayout>())
      ->SetOrientation(views::LayoutOrientation::kHorizontal);

  // contents has title/label and buttons.
  auto* banner_contents =
      banner_type_container_->AddChildView(std::make_unique<views::View>());
  banner_contents->SetLayoutManager(std::make_unique<views::FlexLayout>())
      ->SetOrientation(views::LayoutOrientation::kVertical);
  banner_contents->SetProperty(
      views::kFlexBehaviorKey,
      views::FlexSpecification(views::MinimumFlexSizeRule::kScaleToZero,
                               views::MaximumFlexSizeRule::kUnbounded)
          .WithOrder(2));
  banner_contents->SetProperty(views::kMarginsKey,
                               gfx::Insets::TLBR(16, 16, 0, 0));

  const std::u16string title_label =
      brave_l10n::GetLocalizedResourceUTF16String(
          IDS_BRAVE_SEARCH_CONVERSION_PROMOTION_BANNER_TITLE);
  views::Label::CustomFont title_font = {
      GetFont(16, gfx::Font::Weight::SEMIBOLD)};
  auto* banner_title = banner_contents->AddChildView(
      std::make_unique<views::Label>(title_label, title_font));
  banner_title->SetAutoColorReadabilityEnabled(false);
  banner_title->SetHorizontalAlignment(gfx::ALIGN_LEFT);

  const std::u16string desc_label = brave_l10n::GetLocalizedResourceUTF16String(
      IDS_BRAVE_SEARCH_CONVERSION_PROMOTION_BANNER_DESC);
  views::Label::CustomFont desc_font = {GetFont(14, gfx::Font::Weight::NORMAL)};
  banner_type_description_ = banner_contents->AddChildView(
      std::make_unique<views::Label>(desc_label, desc_font));
  banner_type_description_->SetProperty(views::kMarginsKey,
                                        gfx::Insets::TLBR(4, 0, 0, 0));
  banner_type_description_->SetAutoColorReadabilityEnabled(false);
  banner_type_description_->SetHorizontalAlignment(gfx::ALIGN_LEFT);

  auto* button_row =
      banner_contents->AddChildView(std::make_unique<views::View>());
  button_row->SetProperty(
      views::kFlexBehaviorKey,
      views::FlexSpecification(views::MinimumFlexSizeRule::kScaleToZero,
                               views::MaximumFlexSizeRule::kPreferred));
  button_row->SetProperty(views::kMarginsKey, gfx::Insets::TLBR(12, 0, 0, 0));
  button_row->SetLayoutManager(std::make_unique<views::FlexLayout>())
      ->SetOrientation(views::LayoutOrientation::kHorizontal);
  auto* try_button = button_row->AddChildView(
      std::make_unique<TryButton>(views::Button::PressedCallback(
          base::BindRepeating(&BraveSearchConversionPromotionView::OpenMatch,
                              base::Unretained(this)))));
  try_button->SetProperty(
      views::kFlexBehaviorKey,
      views::FlexSpecification(views::MinimumFlexSizeRule::kScaleToZero,
                               views::MaximumFlexSizeRule::kPreferred));
  try_button->SetProminent(true);
  try_button->SetText(brave_l10n::GetLocalizedResourceUTF16String(
      IDS_BRAVE_SEARCH_CONVERSION_TRY_BUTTON_LABEL));
  try_button->SetFontSize(14);

  auto* dismiss_button = button_row->AddChildView(
      std::make_unique<BannerTypeDismissButton>(views::Button::PressedCallback(
          base::BindRepeating(&BraveSearchConversionPromotionView::Dismiss,
                              base::Unretained(this)))));
  dismiss_button->SetProperty(
      views::kFlexBehaviorKey,
      views::FlexSpecification(views::MinimumFlexSizeRule::kScaleToZero,
                               views::MaximumFlexSizeRule::kPreferred));
  dismiss_button->SetText(brave_l10n::GetLocalizedResourceUTF16String(
      IDS_BRAVE_SEARCH_CONVERSION_DISMISS_BUTTON_LABEL));
  dismiss_button->SetFontSize(14);
  dismiss_button->SetTextColor(views::Button::STATE_NORMAL,
                               SkColorSetRGB(0xff, 0x76, 0x54));
  dismiss_button->SetTextColor(views::Button::STATE_PRESSED,
                               SkColorSetRGB(0xff, 0x76, 0x54));
  dismiss_button->SetTextColor(views::Button::STATE_HOVERED,
                               SkColorSetRGB(0xff, 0x97, 0x7d));
  dismiss_button->SetProperty(views::kMarginsKey,
                              gfx::Insets::TLBR(0, 13, 0, 0));
  dismiss_button->SetTooltipText(brave_l10n::GetLocalizedResourceUTF16String(
      IDS_BRAVE_SEARCH_CONVERSION_DISMISS_BUTTON_TOOLTIP));
}

std::unique_ptr<views::Background>
BraveSearchConversionPromotionView::GetButtonTypeBackground() {
  SkColor bg = gfx::kPlaceholderColor;
  if (auto* tp = GetThemeProvider()) {
    auto bg_color_id = BraveThemeProperties::
        COLOR_SEARCH_CONVERSION_BUTTON_TYPE_BACKGROUND_NORMAL;
    if (IsMouseHovered()) {
      bg_color_id = BraveThemeProperties::
          COLOR_SEARCH_CONVERSION_BUTTON_TYPE_BACKGROUND_HOVERED;
    }
    bg = tp->GetColor(bg_color_id);
  }

  return views::CreateSolidBackground(bg);
}

gfx::Size BraveSearchConversionPromotionView::CalculatePreferredSize() const {
  const int height = type_ == ConversionType::kButton ? 66 : 132;
  return gfx::Size(0, height);
}

void BraveSearchConversionPromotionView::OnThemeChanged() {
  View::OnThemeChanged();

  UpdateState();
}

void BraveSearchConversionPromotionView::UpdateHoverState() {
  UpdateState();
}

BEGIN_METADATA(BraveSearchConversionPromotionView, views::View)
END_METADATA
