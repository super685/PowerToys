// Copyright (c) Microsoft Corporation
// The Microsoft Corporation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

using System.Windows;
using System.Windows.Controls;
using FancyZonesEditor.Models;
using FancyZonesEditor.Utils;

namespace FancyZonesEditor
{
    /// <summary>
    /// Interaction logic for CanvasEditor.xaml
    /// </summary>
    public partial class CanvasEditor : UserControl
    {
        // Non-localizable strings
        private const string PropertyUpdateLayoutID = "UpdateLayout";

        private CanvasLayoutModel _model;

        public CanvasEditor()
        {
            InitializeComponent();
            Loaded += OnLoaded;
        }

        private void OnLoaded(object sender, RoutedEventArgs e)
        {
            CanvasLayoutModel model = (CanvasLayoutModel)DataContext;
            if (model != null)
            {
                _model = model;
                UpdateZoneRects();

                model.PropertyChanged += OnModelChanged;
            }
        }

        private void OnModelChanged(object sender, System.ComponentModel.PropertyChangedEventArgs e)
        {
            if (e.PropertyName == PropertyUpdateLayoutID)
            {
                UpdateZoneRects();
            }
        }

        private void UpdateZoneRects()
        {
            var workArea = App.Overlay.WorkArea;
            Preview.Width = workArea.Width;
            Preview.Height = workArea.Height;

            UIElementCollection previewChildren = Preview.Children;
            int previewChildrenCount = previewChildren.Count;
            while (previewChildrenCount < _model.Zones.Count)
            {
                CanvasZone zone = new CanvasZone
                {
                    Model = _model,
                };

                Preview.Children.Add(zone);
                previewChildrenCount++;
            }

            while (previewChildrenCount > _model.Zones.Count)
            {
                Preview.Children.RemoveAt(previewChildrenCount - 1);
                previewChildrenCount--;
            }

            for (int i = 0; i < previewChildrenCount; i++)
            {
                Int32Rect rect = _model.Zones[i];
                CanvasZone zone = previewChildren[i] as CanvasZone;

                zone.ZoneIndex = i;
                Canvas.SetLeft(zone, rect.X);
                Canvas.SetTop(zone, rect.Y);
                zone.Height = rect.Height;
                zone.Width = rect.Width;
                zone.LabelID.Content = i + 1;
            }
        }
    }
}
